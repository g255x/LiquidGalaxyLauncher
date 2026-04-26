#ifndef CONFIG
#define CONFIG
#include"INCLUDE.h"
/**
 * 操作 配置文件
 * @param mode 模式：1=写入，2=读取，3=删除
 * @param key  要操作的键名
 * @param value 仅在 mode=1 时使用，要写入的值
 * @return mode=2 时返回对应的值（不存在则返回空字符串）；否则返回空字符串
 */
string config(string filename, int mode, std::string key, std::string value = "") {
	filename += ".config";

	if (mode == 1) { // WRITE
		std::vector<std::pair<std::string, std::string>> entries;
		// 读取现有文件
		std::ifstream infile(filename);
		if (infile.is_open()) {
			std::string line;
			while (std::getline(infile, line)) {
				if (line.empty()) continue;
				std::istringstream iss(line);
				std::string k, v;
				if (std::getline(iss, k, ',') && std::getline(iss, v)) {
					entries.emplace_back(k, v);
				}
			}
			infile.close();
		}

		// ADD
		bool found = false;
		for (auto& entry : entries) {
			if (entry.first == key) {
				entry.second = value;
				found = true;
				break;
			}
		}
		if (!found) {
			entries.emplace_back(key, value);
		}

		std::ofstream outfile(filename);
		if (!outfile.is_open()) return "";
		for (const auto& entry : entries) {
			outfile << entry.first << "," << entry.second << "\n";
		}
		outfile.close();
		return "";
	} else if (mode == 2) { // READ
		std::ifstream infile(filename);
		if (!infile.is_open()) return "";
		std::string line;
		while (std::getline(infile, line)) {
			if (line.empty()) continue;
			std::istringstream iss(line);
			std::string k, v;
			if (std::getline(iss, k, ',') && std::getline(iss, v)) {
				if (k == key) {
					infile.close();
					return v;
				}
			}
		}
		infile.close();
		return "error";
	} else if (mode == 3) { // DEL
		std::vector<std::pair<std::string, std::string>> entries;
		std::ifstream infile(filename);
		if (infile.is_open()) {
			std::string line;
			while (std::getline(infile, line)) {
				if (line.empty()) continue;
				std::istringstream iss(line);
				std::string k, v;
				if (std::getline(iss, k, ',') && std::getline(iss, v)) {
					if (k != key) {
						entries.emplace_back(k, v);
					}
				}
			}
			infile.close();
		}

		// RE
		std::ofstream outfile(filename);
		if (!outfile.is_open()) return "";
		for (const auto& entry : entries) {
			outfile << entry.first << "," << entry.second << "\n";
		}
		outfile.close();
		return "";
	} else {
		return "";
	}
}
#endif
