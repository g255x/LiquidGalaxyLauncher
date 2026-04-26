#include"INCLUDE.h"
#include"achieve.h"
#ifndef MOD
#define MOD
namespace fs = std::filesystem;

size_t utf8_length(const std::string& s) {
	size_t len = 0;
	for (size_t i = 0; i < s.size(); ++i) {
		unsigned char c = s[i];
		if ((c & 0xC0) != 0x80) ++len;
	}
	return len;
}

std::string utf8_substr(const std::string& s, size_t n) {
	if (n == 0) return "";
	size_t cnt = 0, pos = 0;
	for (size_t i = 0; i < s.size(); ++i) {
		unsigned char c = s[i];
		if ((c & 0xC0) != 0x80) {
			if (cnt == n) break;
			++cnt;
		}
		++pos;
	}
	return s.substr(0, pos);
}

void LprintColoredString(const std::string& content, const std::string& hex_color,
                         int width, bool right_align = false) {
	if (width <= 0) {
		std::cout << content;
		return;
	}

	int r = 0, g = 0, b = 0;
	if (hex_color.size() == 7 && hex_color[0] == '#') {
		r = std::stoi(hex_color.substr(1, 2), nullptr, 16);
		g = std::stoi(hex_color.substr(3, 2), nullptr, 16);
		b = std::stoi(hex_color.substr(5, 2), nullptr, 16);
	}
	std::string color_code = "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";

	size_t len = utf8_length(content);
	std::string final_str;
	if (len > static_cast<size_t>(width)) {
		size_t keep = (width > 1) ? width - 1 : 0;
		final_str = utf8_substr(content, keep) + "...";
	} else {
		// Моід
		if (right_align) {
			final_str = std::string(width - len, ' ') + content;
		} else {
			final_str = content + std::string(width - len, ' ');
		}
	}
	std::cout << color_code << final_str << "\033[0m";
}

std::string to_lower(const std::string& s) {
	std::string res = s;
	std::transform(res.begin(), res.end(), res.begin(), ::tolower);
	return res;
}

std::string extract_mod_name(const std::string& filename) {
	std::string stem = filename;
	if (stem.size() >= 4 && stem.substr(stem.size() - 4) == ".jar")
		stem = stem.substr(0, stem.size() - 4);

	if (!stem.empty() && stem[0] == '[') {
		size_t end = stem.find(']');
		if (end != std::string::npos) {
			stem = stem.substr(end + 1);
			while (!stem.empty() && stem[0] == ' ') stem.erase(0, 1);
		}
	}

	std::string lower_stem = to_lower(stem);
	const char* suffixes[] = {"fabric", "forge", "quilt", "neoforge", "rift", "liteloader"};
	bool changed = true;
	int iter = 0;
	while (changed && iter < 10) {
		changed = false;
		++iter;
		size_t last_sep = stem.find_last_of("-_");
		if (last_sep == std::string::npos) break;
		std::string tail = stem.substr(last_sep + 1);
		std::string lower_tail = to_lower(tail);
		bool removed = false;
		for (const char* suf : suffixes) {
			if (lower_tail == suf) {
				stem = stem.substr(0, last_sep);
				removed = true;
				break;
			}
		}
		if (removed) {
			changed = true;
			continue;
		}
		if (!tail.empty() && (std::isdigit(tail[0]) || (tail[0] == 'v' && tail.size() > 1 && std::isdigit(tail[1])))) {
			bool is_version = true;
			for (char c : tail) {
				if (c == 'v' || c == '.' || c == '-' || c == '_') continue;
				if (!std::isdigit(c) && !std::isalpha(c)) {
					is_version = false;
					break;
				}
			}
			if (is_version) {
				stem = stem.substr(0, last_sep);
				changed = true;
				continue;
			}
		}
		if (lower_tail.find("mc") == 0 && lower_tail.size() > 2 && std::isdigit(lower_tail[2])) {
			stem = stem.substr(0, last_sep);
			changed = true;
			continue;
		}
	}
	if (stem.empty() || stem.size() < 2) {
		size_t pos = stem.find_first_of("-_.");
		if (pos != std::string::npos) stem = stem.substr(0, pos);
	}
	std::replace(stem.begin(), stem.end(), '.', '-');
	std::replace(stem.begin(), stem.end(), '_', '-');
	while (!stem.empty() && stem.front() == '-') stem.erase(0, 1);
	while (!stem.empty() && stem.back() == '-') stem.pop_back();
	if (stem.empty()) {
		std::string base = filename;
		if (base.size() >= 4 && base.substr(base.size() - 4) == ".jar") base = base.substr(0, base.size() - 4);
		size_t pos = base.find_first_of("-_.");
		if (pos != std::string::npos) stem = base.substr(0, pos);
		else stem = base;
	}
	return stem;
}

bool mod() {
	fs::path mods_dir = "mods";
	if (!fs::exists(mods_dir) || !fs::is_directory(mods_dir)) {
		printInformation(3, "The directory does not exist or cannot be accessed://mods");
		return false;
	}

	std::vector<std::pair<std::string, std::string>> mod_list;
	for (const auto& entry : fs::directory_iterator(mods_dir)) {
		if (entry.is_regular_file() && entry.path().extension() == ".jar") {
			std::string filename = entry.path().filename().string();
			std::string mod_name = extract_mod_name(filename);
			mod_list.emplace_back(mod_name, filename);
		}
	}
	if (mod_list.empty()) {
		printInformation(2, "No Mods!");
		return true;
	}

	std::sort(mod_list.begin(), mod_list.end(),
	[](const auto & a, const auto & b) {
		return a.first < b.first;
	});

	const int col_idx   = 5;
	const int col_name  = 25;
	const int col_file  = -1;
	LprintColoredString("Num", "#9370db", col_idx, false);
	std::cout << " | ";
	LprintColoredString("Mod Name", "#9370db", col_name, false);
	std::cout << " | ";
	LprintColoredString("JAR Name", "#dda0dd", col_file, false);
	std::cout << '\n';

	int line_len = col_idx + col_name + col_file + 2 * 3;
	for (int i = 0; i < line_len; ++i) std::cout << '-';
	std::cout << '\n';

	int idx = 1;
	for (const auto& [mod_name, orig_name] : mod_list) {
		std::string idx_str = std::to_string(idx++);
		LprintColoredString(idx_str, "#9370db", col_idx, true);
		std::cout << " | ";
		LprintColoredString(mod_name, "#9370db", col_name, false);
		std::cout << " | ";
		LprintColoredString(orig_name, "#dda0dd", col_file, false);
		std::cout << '\n';
	}
	return true;
}
#endif
