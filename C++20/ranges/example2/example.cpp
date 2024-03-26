// compile with g++-13
// $ CXX=g++-13 make

#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

struct Book {
	std::string title;
	std::string author;
	int id;
	auto operator<=>(const auto &b) const { return author <=> b.author; }
};

class Database {
  public:
	Database() = default;
	Database(const Database &b) = delete;
	Database(Database &&b) : db{std::move(b.db)}, index{b.index} {}
	Database &operator=(const Database &b) = delete;
	Database &operator=(Database &&b) {
		db = std::move(b.db);
		index = b.index;
		return *this;
	}
	void addItem(std::string_view title, std::string_view author) {
		index++;
		auto &pos = db[index];
		pos.id = index;
		pos.title = title;
		pos.author = author;
	}
	int findItemByTitle(std::string_view title) {
		for (const auto &i : db) {
			if (i.second.title == title)
				return i.first;
		}
		return -1;
	}

	int findItemByAuthor(std::string_view author) {
		for (const auto &i : db) {
			if (i.second.author == author)
				return i.first;
		}
		return -1;
	}
	Book &lookup(int id) { return db[id]; }
	Book &operator[](const int &pos) { return db[pos]; }
	auto &database() { return db; }

	void printSorted(std::ostream &out) {
		std::vector<std::string> b;
		for (const auto &i : std::views::all(db)) {
			b.push_back(i.second.author + ", [ " + i.second.title + " ] ");
		}
		std::ranges::sort(b);
		for (const auto &i : std::views::all(b)) {
			std::cout << i << "\n";
		}
	}

  private:
	std::unordered_map<int, Book> db;
	int index;
};

int main(int argc, char **argv) {
	Database db;
	db.addItem("C++ Programming Language", "Stroustrup, Bjarne");
	db.addItem("The Dead Zone", "King, Stephen");
	db.addItem("Night Shift", "King, Stephen");
	db.printSorted(std::cout);
	int id = db.findItemByTitle("The Dead Zone");
	if (id != -1) {
		std::cout << "by " << db[id].author << "\n";
	}
	for (const auto &i : std::views::take(db.database(), 3)) {
		std::cout << i.second.id << "\n";
	}
	return 0;
}