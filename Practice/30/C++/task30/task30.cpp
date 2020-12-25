#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <Windows.h>

struct Item {
	enum class ItemTypeNames { COIN, RUNE, WEAPON, ARMOR } ItemTypeName;
	union ItemTypes {
		struct Coin{
			short count; 
		} coin;
		struct Rune{
			short level;
			enum Elements { FIRE, WATER, EARTH, AIR } element;
		} rune;
		struct Weapon{
			short damage;
			short critical;
			short durability;
		} weapon;
		struct Armor{
			short guard;
			short durability;
		} armor;
	} ItemType;
};

Item RandomItem();
void Col(int text);
std::vector<int> the10000points;     // Вектор на 10 000 точек (для реализации вероятностей)
typedef std::vector<Item> LootBox;   // Псевдоним для вектора с элементами Item

enum Colors {
	BLACK,
	DARK_BLUE,
	GREEN,
	LIGHT_BLUE,
	RED,
	VIOLET,
	YELLOW,
	DEFAULT,
	GRAY,
	SALMON = 12,
	PURPLE,
	PALE_YELLOW
};

// Вывод содержимого переменной типа Item
std::ostream& operator<< (std::ostream& out, Item item) {
	switch (item.ItemTypeName)
	{
	case (Item::ItemTypeNames::COIN):
		Col(YELLOW);
		std::cout << item.ItemType.coin.count << " gold\n";
		Col(DEFAULT);
		break;
	case (Item::ItemTypeNames::RUNE):
		Col(VIOLET);
		std::cout << "Rune";
		Col(DEFAULT);
		std::cout << " of ";
		switch (item.ItemType.rune.element)
		{
		case (Item::ItemTypes::Rune::Elements::AIR):
			Col(LIGHT_BLUE);
			std::cout << "AIR ";
			break;
		case(Item::ItemTypes::Rune::Elements::EARTH):
			Col(GREEN);
			std::cout << "EARTH ";
			break;
		case(Item::ItemTypes::Rune::Elements::WATER):
			Col(DARK_BLUE);
			std::cout << "WATER ";
			break;
		case(Item::ItemTypes::Rune::Elements::FIRE):
			Col(RED);
			std::cout << "FIRE ";
			break;
		}
		switch (item.ItemType.rune.level)
		{
		case(1):
		case(2):
		case(3):
		case(4):
			Col(DEFAULT);
			std::cout << "level " << item.ItemType.rune.level << std::endl;
			break;
		case(5):
		case(6):
		case(7):
		case(8):
		case(9):
			Col(PURPLE);
			std::cout << "level " << item.ItemType.rune.level << std::endl;
			break;
		case(10):
			Col(PALE_YELLOW);
			std::cout << "level " << item.ItemType.rune.level << std::endl;
			break;
		}
		Col(DEFAULT);
		break;
	case (Item::ItemTypeNames::WEAPON):
		if ((item.ItemType.weapon.damage == 100) && (item.ItemType.weapon.critical == 0)) {
			Col(PALE_YELLOW);
			std::cout << "God Killer\n";
			Col(DEFAULT);
		}
		else {
			if ((item.ItemType.weapon.damage == 75) && (item.ItemType.weapon.critical == 50)) {
				Col(PURPLE);
				std::cout << "Demon Slayer\n";
				Col(DEFAULT);
			}
			else {
				Col(RED);
				std::cout << "Weapon:\n";
				Col(DEFAULT);
				std::cout << "   damage: " << item.ItemType.weapon.damage << "\n";
				std::cout << "   critical: " << item.ItemType.weapon.critical << "%\n";
				std::cout << "   durability:  " << item.ItemType.weapon.durability << "%\n";
			}
		}
		break;
	case (Item::ItemTypeNames::ARMOR):
		if (item.ItemType.armor.guard == 50) {
			Col(GRAY);
			std::cout << "Bronezhiletka\n";
			Col(DEFAULT);
		}
		else {
			Col(GRAY);
			std::cout << "Armor:\n";
			Col(DEFAULT);
			std::cout << "   guard: " << item.ItemType.armor.guard << "\n";
			std::cout << "   durability: " << item.ItemType.armor.durability << "%\n";
		}
		break;
	};
	return out;
}

// Вывод содержимого LootBox-а
std::ostream& operator<< (std::ostream& out, LootBox box) {
	for (int i = 0; i < size(box); i++) {
		std::cout << box[i];
	}
	return out;
}

enum TableItems {
	ITEM0,   // Coin	count = 1000
	ITEM1,   // Rune	level = 1; element = FIRE
	ITEM2,   // Rune	level = 1; element = WATER
	ITEM3,   // Rune	level = 1; element = EARTH
	ITEM4,   // Rune	level = 1; element = AIR
	ITEM5,   // Rune	level = 5; element = FIRE
	ITEM6,   // Rune	level = 5; element = WATER
	ITEM7,   // Rune	level = 5; element = EARTH
	ITEM8,   // Rune	level = 5; element = AIR
	ITEM9,   // Rune	level = 10; element = FIRE
	ITEM10,   // Rune	level = 10; element = WATER
	ITEM11,   // Rune	level = 10; element = EARTH
	ITEM12,   // Rune	level = 10; element = AIR
	ITEM13,   // Weapon	damage = 100; critical = 0; durability = 100
	ITEM14,   // Weapon	damage = 75; critical = 50; durability = 100
	ITEM15,   // Armor	guard = 50; durability = 100
};

// Изменяет содержимое переменной Item на случайное
void operator++ (Item& NewItem) {
	int drop = (rand() * rand()) % 10000;
	switch (the10000points[drop])
	{
	case(ITEM0):
		NewItem.ItemTypeName = Item::ItemTypeNames::COIN;
		NewItem.ItemType.coin.count = 1000;
		break;
	case(ITEM1):
		NewItem.ItemTypeName = Item::ItemTypeNames::RUNE;
		NewItem.ItemType.rune.level = 1;
		NewItem.ItemType.rune.element = Item::ItemTypes::Rune::Elements::FIRE;
		break;
	case(ITEM2):
		NewItem.ItemTypeName = Item::ItemTypeNames::RUNE;
		NewItem.ItemType.rune.level = 1;
		NewItem.ItemType.rune.element = Item::ItemTypes::Rune::Elements::WATER;
		break;
	case(ITEM3):
		NewItem.ItemTypeName = Item::ItemTypeNames::RUNE;
		NewItem.ItemType.rune.level = 1;
		NewItem.ItemType.rune.element = Item::ItemTypes::Rune::Elements::EARTH;
		break;
	case(ITEM4):
		NewItem.ItemTypeName = Item::ItemTypeNames::RUNE;
		NewItem.ItemType.rune.level = 1;
		NewItem.ItemType.rune.element = Item::ItemTypes::Rune::Elements::AIR;
		break;
	case(ITEM5):
		NewItem.ItemTypeName = Item::ItemTypeNames::RUNE;
		NewItem.ItemType.rune.level = 5;
		NewItem.ItemType.rune.element = Item::ItemTypes::Rune::Elements::FIRE;
		break;
	case(ITEM6):
		NewItem.ItemTypeName = Item::ItemTypeNames::RUNE;
		NewItem.ItemType.rune.level = 5;
		NewItem.ItemType.rune.element = Item::ItemTypes::Rune::Elements::WATER;
		break;
	case(ITEM7):
		NewItem.ItemTypeName = Item::ItemTypeNames::RUNE;
		NewItem.ItemType.rune.level = 5;
		NewItem.ItemType.rune.element = Item::ItemTypes::Rune::Elements::EARTH;
		break;
	case(ITEM8):
		NewItem.ItemTypeName = Item::ItemTypeNames::RUNE;
		NewItem.ItemType.rune.level = 5;
		NewItem.ItemType.rune.element = Item::ItemTypes::Rune::Elements::AIR;
		break;
	case(ITEM9):
		NewItem.ItemTypeName = Item::ItemTypeNames::RUNE;
		NewItem.ItemType.rune.level = 10;
		NewItem.ItemType.rune.element = Item::ItemTypes::Rune::Elements::FIRE;
		break;
	case(ITEM10):
		NewItem.ItemTypeName = Item::ItemTypeNames::RUNE;
		NewItem.ItemType.rune.level = 10;
		NewItem.ItemType.rune.element = Item::ItemTypes::Rune::Elements::WATER;
		break;
	case(ITEM11):
		NewItem.ItemTypeName = Item::ItemTypeNames::RUNE;
		NewItem.ItemType.rune.level = 10;
		NewItem.ItemType.rune.element = Item::ItemTypes::Rune::Elements::EARTH;
		break;
	case(ITEM12):
		NewItem.ItemTypeName = Item::ItemTypeNames::RUNE;
		NewItem.ItemType.rune.level = 10;
		NewItem.ItemType.rune.element = Item::ItemTypes::Rune::Elements::AIR;
		break;
	case(ITEM13):
		NewItem.ItemTypeName = Item::ItemTypeNames::WEAPON;
		NewItem.ItemType.weapon.damage = 100;
		NewItem.ItemType.weapon.critical = 0;
		NewItem.ItemType.weapon.durability = 100;
		break;
	case(ITEM14):
		NewItem.ItemTypeName = Item::ItemTypeNames::WEAPON;
		NewItem.ItemType.weapon.damage = 75;
		NewItem.ItemType.weapon.critical = 50;
		NewItem.ItemType.weapon.durability = 100;
		break;
	case(ITEM15):
		NewItem.ItemTypeName = Item::ItemTypeNames::ARMOR;
		NewItem.ItemType.armor.guard = 50;
		NewItem.ItemType.armor.durability = 100;
		break;
	};
}

// Добавляет Item в LootBox
LootBox& operator<< (LootBox& box, Item item) {
	box.push_back(item);
	return box;
}

// Возвращает лутбокс с 3 случайными предметами
LootBox GenerateLootBox() {
	LootBox box;
	Item NewItem;
	for (int i = 0; i < 3; i++) {
		++NewItem;
		box << NewItem;
	}
	return box;
}

// Заменяет в строке большие буквы на маленькие
std::string& form_str(std::string& s) {
	int r = 'A' - 'a';
	for (int i = 0; i < size(s); i++) {
		if ((s[i] >= 'A') && (s[i] <= 'Z')) {
			s[i] = s[i] - r;
		}
	}
	return s;
}

int main()
{
	srand(time(0));
	// Заполнение вектора из 10 000 точек
	std::vector<int> probs = { 5000, 600, 1300, 700, 1400, 60, 130, 70, 140, 6, 13, 7, 14, 140, 140, 280 };  // вероятности всех предметов в таблице умноженные на 100
	int item_number = 0;
	for (int p : probs) {
		for (int i = 0; i < p; i++) {
			the10000points.push_back(item_number);
		}
		item_number++;
	}

	LootBox box;
	std::string action = "";
	while (action != "no") {
		std::cout << "Open a lootbox? Yes/No (R - get random item; I - show inventory)\n";
		std::cin >> action;
		form_str(action);
		if ((action == "yes") || (action == "y")) {
			box = GenerateLootBox();
			std::cout << box << std::endl;
			continue;
		}
		if (action == "r") {
			Item NewItem;
			NewItem = RandomItem();
			std::cout << NewItem << std::endl;
			continue;
		}
		if (action == "i") {
			std::cout << "This function is not avable yet.\n";
			std::cout << std::endl;
			continue;
		}
		if ((action == "no") || (action == "n")) {
			std::cout << "Goodbye!\n";
			return 0;
		}
		std::cout << std::endl;
	}
	return 0;
}

// Просто функция (возвращает случайный предмет со случайными характеристиками)
Item RandomItem() {
	Item NewItem;
	NewItem.ItemTypeName = static_cast<Item::ItemTypeNames>(rand() % 4);
	switch (NewItem.ItemTypeName)
	{
	case (Item::ItemTypeNames::COIN):
		NewItem.ItemType.coin.count = rand() % 1001;
		break;
	case (Item::ItemTypeNames::RUNE):
		NewItem.ItemType.rune.level = (rand() % 10) + 1;
		NewItem.ItemType.rune.element = static_cast<Item::ItemTypes::Rune::Elements>(rand() % 4);
		break;
	case (Item::ItemTypeNames::WEAPON):
		NewItem.ItemType.weapon.damage = (rand() % 512) + 1;
		NewItem.ItemType.weapon.critical = (rand() % 101);
		NewItem.ItemType.weapon.durability = (rand() % 100) + 1;
		break;
	case (Item::ItemTypeNames::ARMOR):
		NewItem.ItemType.armor.guard = (rand() % 512) + 1;
		NewItem.ItemType.armor.durability = (rand() % 100) + 1;
		break;
	};
	return NewItem;
}

// Функция устанавливает цвет текста для вывода в консоль
void Col(int text)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((0 << 4) | text));
}