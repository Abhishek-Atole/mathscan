/*
 * Project Cleanup Utility
 *
 * Objective:
 * - Recursively scan the project folder.
 * - Identify and delete unwanted files such as:
 *    - Temporary files (*.tmp, *.bak, *~, *.swp)
 *    - Build output directories (e.g., "build", "debug", "release")
 *    - Other unwanted artifacts (e.g., .DS_Store, Thumbs.db)
 * - Preserve all source (*.cpp, *.h, *.hpp) and project files.
 * - Provide console output listing deleted files.
 * - Handle errors gracefully and safely.
 * - Written in portable C++17 or later.
 *
 * Usage:
 * Run this utility from the project root directory.
 */

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class ProjectCleanup {
   private:
    size_t deletedFiles = 0;
    size_t deletedDirs = 0;
    size_t totalSize = 0;
    bool dryRun = false;

    // Define unwanted file patterns
    static const std::vector<std::string> unwantedExtensions;
    static const std::vector<std::string> unwantedDirectories;
    static const std::vector<std::string> unwantedFiles;

   public:
    bool isUnwantedFile(const fs::path& path) {
        std::string filename = path.filename().string();
        std::string extension = path.extension().string();

        // Transform extension to lowercase for case-insensitive comparison
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

        // Check unwanted extensions
        for (const auto& ext : unwantedExtensions) {
            if (extension == ext) {
                return true;
            }
        }

        // Check unwanted file patterns (like files ending with ~)
        for (const auto& pattern : unwantedFiles) {
            if (filename.size() >= pattern.size() &&
                filename.compare(filename.size() - pattern.size(), pattern.size(), pattern) == 0) {
                return true;
            }
        }

        // Check exact unwanted filenames
        std::string lowerFilename = filename;
        std::transform(lowerFilename.begin(), lowerFilename.end(), lowerFilename.begin(),
                       ::tolower);

        if (lowerFilename == ".ds_store" || lowerFilename == "thumbs.db" ||
            lowerFilename == "desktop.ini" || filename == ".gitignore.bak") {
            return true;
        }

        return false;
    }

    bool isUnwantedDirectory(const fs::path& path) {
        if (!fs::is_directory(path)) {
            return false;
        }

        std::string dirname = path.filename().string();
        std::transform(dirname.begin(), dirname.end(), dirname.begin(), ::tolower);

        for (const auto& dirName : unwantedDirectories) {
            if (dirname == dirName) {
                return true;
            }
        }

        return false;
    }

    size_t getFileSize(const fs::path& path) {
        try {
            if (fs::is_regular_file(path)) {
                return fs::file_size(path);
            } else if (fs::is_directory(path)) {
                size_t size = 0;
                for (const auto& entry : fs::recursive_directory_iterator(path)) {
                    if (fs::is_regular_file(entry)) {
                        size += fs::file_size(entry);
                    }
                }
                return size;
            }
        } catch (const fs::filesystem_error&) {
            // Ignore errors when calculating size
        }
        return 0;
    }

    void cleanDirectory(const fs::path& dir, bool isRoot = false) {
        if (!fs::exists(dir) || !fs::is_directory(dir)) {
            std::cerr << "Directory does not exist: " << dir << "\n";
            return;
        }

        std::vector<fs::path> itemsToDelete;

        // First pass: collect items to delete
        try {
            for (const auto& entry : fs::directory_iterator(dir)) {
                const fs::path& path = entry.path();

                if (isUnwantedDirectory(path) || isUnwantedFile(path)) {
                    itemsToDelete.push_back(path);
                } else if (fs::is_directory(path) && !isUnwantedDirectory(path)) {
                    // Recursively clean subdirectories (but don't delete them)
                    cleanDirectory(path, false);
                }
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error reading directory " << dir << ": " << e.what() << "\n";
            return;
        }

        // Second pass: delete collected items
        for (const auto& path : itemsToDelete) {
            try {
                size_t itemSize = getFileSize(path);

                if (fs::is_directory(path)) {
                    if (dryRun) {
                        std::cout << "◇ Would delete directory: " << path << " ("
                                  << formatSize(itemSize) << ")\n";
                    } else {
                        fs::remove_all(path);
                        std::cout << "✓ Deleted directory: " << path << " (" << formatSize(itemSize)
                                  << ")\n";
                    }
                    deletedDirs++;
                } else {
                    if (dryRun) {
                        std::cout << "◇ Would delete file: " << path << " (" << formatSize(itemSize)
                                  << ")\n";
                    } else {
                        fs::remove(path);
                        std::cout << "✓ Deleted file: " << path << " (" << formatSize(itemSize)
                                  << ")\n";
                    }
                    deletedFiles++;
                }

                totalSize += itemSize;
            } catch (const fs::filesystem_error& e) {
                std::cerr << "✗ Error " << (dryRun ? "checking" : "deleting") << " " << path << ": "
                          << e.what() << "\n";
            }
        }
    }

    std::string formatSize(size_t bytes) {
        const char* units[] = {"B", "KB", "MB", "GB"};
        int unit = 0;
        double size = static_cast<double>(bytes);

        while (size >= 1024 && unit < 3) {
            size /= 1024;
            unit++;
        }

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << size << " " << units[unit];
        return oss.str();
    }

    void printHeader() {
        std::cout << "╔══════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                    Project Cleanup Utility                  ║\n";
        std::cout << "║                      MathScan Project                        ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════╝\n\n";
    }

    void printSummary() {
        std::cout << "\n" << std::string(60, '=') << "\n";
        std::cout << "CLEANUP SUMMARY:\n";
        std::cout << std::string(60, '=') << "\n";
        std::cout << "Files deleted:       " << deletedFiles << "\n";
        std::cout << "Directories deleted: " << deletedDirs << "\n";
        std::cout << "Total space freed:   " << formatSize(totalSize) << "\n";
        std::cout << std::string(60, '=') << "\n";
    }

    void run(bool isDryRun = false) {
        dryRun = isDryRun;
        printHeader();

        fs::path projectRoot = fs::current_path();
        std::cout << "Starting cleanup in project directory: " << projectRoot << "\n";

        if (dryRun) {
            std::cout << "*** DRY RUN MODE - No files will be deleted ***\n";
        }

        std::cout << "Scanning for unwanted files and directories...\n\n";

        auto startTime = std::chrono::high_resolution_clock::now();

        cleanDirectory(projectRoot, true);

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        printSummary();
        std::cout << "Cleanup completed in " << duration.count() << " ms.\n";

        if (deletedFiles == 0 && deletedDirs == 0) {
            std::cout << "\n✓ Project directory is already clean!\n";
        } else if (dryRun) {
            std::cout << "\n✓ Run without --dry-run to actually delete these files.\n";
        }
    }
};

// Static member definitions
const std::vector<std::string> ProjectCleanup::unwantedExtensions = {
    ".tmp",  ".bak", ".swp", ".swo",   ".log", ".cache", ".old",     ".orig",    ".rej",   ".patch",
    ".diff", ".pyc", ".pyo", ".class", ".o",   ".obj",   ".exe.bak", ".dll.bak", ".so.bak"};

const std::vector<std::string> ProjectCleanup::unwantedDirectories = {"build",
                                                                      "debug",
                                                                      "release",
                                                                      ".vs",
                                                                      ".idea",
                                                                      "cmake-build-debug",
                                                                      "cmake-build-release",
                                                                      "__pycache__",
                                                                      ".pytest_cache",
                                                                      "node_modules",
                                                                      ".svn",
                                                                      ".hg",
                                                                      "bin",
                                                                      "obj",
                                                                      "out",
                                                                      "dist",
                                                                      "cmake-build-relwithdebinfo",
                                                                      "cmake-build-minsizerel"};

const std::vector<std::string> ProjectCleanup::unwantedFiles = {"~", ".tmp", ".swp", ".swo"};

int main(int argc, char* argv[]) {
    try {
        // Check if user wants help
        if (argc > 1 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")) {
            std::cout << "Project Cleanup Utility\n";
            std::cout << "Usage: " << argv[0] << " [options]\n\n";
            std::cout << "Options:\n";
            std::cout << "  --help, -h    Show this help message\n";
            std::cout << "  --dry-run     Show what would be deleted without actually deleting\n\n";
            std::cout << "This utility removes unwanted files and directories including:\n";
            std::cout << "- Temporary files (*.tmp, *.bak, *~, *.swp)\n";
            std::cout << "- Build directories (build, debug, release, etc.)\n";
            std::cout << "- IDE artifacts (.vs, .idea, .vscode)\n";
            std::cout << "- System files (.DS_Store, Thumbs.db)\n\n";
            std::cout << "Source files (*.cpp, *.h, *.hpp) and project files are preserved.\n";
            return 0;
        }

        // Check for dry-run mode
        bool isDryRun = false;
        if (argc > 1 && std::string(argv[1]) == "--dry-run") {
            isDryRun = true;
        }

        ProjectCleanup cleanup;
        cleanup.run(isDryRun);

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred.\n";
        return 1;
    }

    return 0;
}
