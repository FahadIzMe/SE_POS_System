#pragma comment(lib, "winmm.lib")
#include <iostream>
#include <conio.h>
#include <fstream>
#include <windows.h>
#include <vector>
#include <iomanip>
#include <string>
#include <algorithm>

using namespace std;

class Item {

public:
	int id;
	string name;
	int quantity;
	float price;
	Item() {
		id = 0;
		name = "";
		quantity = 0;
		price = 0;
	}
	int getid() {
		return id;
	}
	void setid(int i) {
		id = i;
	}
	string getname() {
		return name;
	}
	void setname(string n) {
		name = n;
	}
	int getquantity() {
		return quantity;
	}
	void setquantity(int q) {
		quantity = q;
	}
	double getprice() {
		return price;
	}
	void setprice(double p) {
		price = p;
	}
	void displayItem() {
		cout << setw(4) << id << setw(20) << name << setw(10) << quantity << setw(10) << price << endl;
	}
};

class Order {
public:
	vector<int> itemId;
	vector<int> quantity;
	string customerName;
	Order() {
		itemId.clear();
		quantity.clear();
		customerName = "";
	}
	void displayOrder() const {
		cout << "Customer Name: " << customerName << endl;
		cout << "Item ID\tQuantity" << endl;
		for (size_t i = 0; i < itemId.size(); ++i) {
			cout << itemId[i] << "\t" << quantity[i] << endl;
		}
		cout << "----------------------------------" << endl;
	}
};


class POSSystem {
public:

	void displayItems(string filename) {
		
		ifstream file(filename);

		if (!file.is_open()) {
			cout << "Error opening file: " << filename << endl;
			return;
		}
		vector<Item> items;
		Item item;
		while (file >> item.id >> item.name >> item.quantity >> item.price) {
			items.push_back(item);
		}

		file.close();
		cout << setw(4) << "ID" << setw(20) << "Name" << setw(10) << "Quantity" << setw(10) << "Price" << endl;
		for (auto& item : items) {
			item.displayItem();
		}
	}

	void processOrder(string filename) 
	{

		ifstream file(filename);
		if (!file.is_open()) {
			cerr << "Error opening file\n";
			return;
		}

		// Read items from the file and store them in a vector
		vector<Item> items;
		Item item;
		while (file >> item.id >> item.name >> item.quantity >> item.price) {
			items.push_back(item);
		}
		file.close();

		// Create a temporary order object
		Order order;

		// Prompt user for the name of the customer
		cout << "Enter customer name: ";
		getline(cin >> ws, order.customerName); // Use getline to include spaces in the name

		// Prompt user to input item IDs
		cout << "Enter item IDs (enter -1 to finish):" << endl;
		while (true) {
			int itemId;
			cout << "Item ID: ";
			cin >> itemId;

			// Check if the user wants to finish
			if (itemId == -1) {
				break;
			}

			// Search for the item in the items vector
			bool itemFound = false;
			for (auto& item : items) {
				if (item.id == itemId) {
					itemFound = true;
					// Input quantity
					cout << "Enter quantity for item " << item.name << ": ";
					int quantity;
					cin >> quantity;

					// Check if there is enough stock
					if (quantity <= item.quantity) {
						// Update quantity in the vector
						item.quantity -= quantity;

						// Add item and quantity to order
						order.itemId.push_back(itemId);
						order.quantity.push_back(quantity);
					}
					else {
						cout << "Not enough stock for item " << item.name << endl;
					}
					break;
				}
			}

			if (!itemFound) {
				cout << "Item " << itemId << " not found" << endl;
			}
		}

		// Display the items and quantities in the order
		cout << "\n\nOrder details:" << endl;
		cout << "Customer Name: " << order.customerName << endl;
		cout << "Item ID\tName\tQuantity\tPrice" << endl;
		float totalBill = 0;
		for (size_t i = 0; i < order.itemId.size(); ++i) {
			for (const auto& item : items) {
				if (item.id == order.itemId[i]) {
					cout << item.id << "\t" << item.name << "\t" << order.quantity[i] << "\t$" << fixed << setprecision(2) << item.price << endl;
					totalBill += item.price * order.quantity[i];
					break;
				}
			}
		}

		// Display total bill
		cout << "Total Bill: $" << fixed << setprecision(2) << totalBill << endl;

		// Write the order details to "Orders.txt" file
		ofstream ordersFile("Orders.txt", ios::app);
		if (!ordersFile.is_open()) {
			cerr << "Error opening Orders.txt file\n";
			return;
		}
		ordersFile << "Order details:" << endl;
		ordersFile << "Customer Name: " << order.customerName << endl;
		ordersFile << "Item ID\tQuantity" << endl;
		for (size_t i = 0; i < order.itemId.size(); ++i) {
			ordersFile << order.itemId[i] << "\t" << order.quantity[i] << endl;
		}
		ordersFile << "----------------------------------" << endl;
		ordersFile.close();
		// Write the updated quantities back to the "records.txt" file
		ofstream updatedFile(filename);
		if (!updatedFile.is_open()) {
			cout << "Error opening records.txt file for updating quantities\n";
			return;
		}
		for (const auto& item : items) {
			updatedFile << item.id << "\t" << item.name << "\t" << item.quantity << "\t" << item.price << endl;
		}
		updatedFile.close();
	}
	
	void updateInventory(string filename) {

		ifstream file1(filename);

		if (!file1.is_open()) {
			cout << "Error opening file: " << filename << endl;
			return;
		}
		vector<Item> items;
		Item item;
		while (file1 >> item.id >> item.name >> item.quantity >> item.price) {
			items.push_back(item);
		}

		file1.close();

		int itemId, newQuantity;
		cout << "Enter the item ID to update its inventory: ";
		cin >> itemId;

		// Find the item in the vector
		for (auto& item : items) {
			if (item.id == itemId) {
				cout << "Enter the new quantity: ";
				cin >> newQuantity;
				item.quantity = max(0, newQuantity); // Ensure the quantity is not less than 0
				break;
			}
		}

		// Rewrite the items file with the updated information
		ofstream file(filename);
		if (!file.is_open()) {
			cerr << "Error opening items file\n";
			return;
		}
		for (const auto& item : items) {
			file << item.id << "\t" << item.name << "\t" << item.quantity << "\t" << item.price << endl;
		}
		file.close();
	}
	

	void showOrders(string filename)
	{
		int numOrders;
		cout << "Enter the number of orders to show: ";
		cin >> numOrders;

		ifstream file(filename);
		if (!file.is_open()) {
			cerr << "Error opening Orders.txt file\n";
			return;
		}

		vector<Order> orders;
		string line;
		Order order;

		// Read orders from the file
		while (getline(file, line)) {
			if (line == "----------------------------------") {
				orders.push_back(order);
				order = Order();
			}
			else if (line.find("Customer Name: ") != string::npos) {
				order.customerName = line.substr(line.find(":") + 2); // Extract customer name
			}
			else if (line.find("\t") != string::npos) {
				size_t tabPos = line.find("\t");
				if (tabPos != string::npos && tabPos < line.size() - 1) {
					try {
						int itemId = stoi(line.substr(0, tabPos)); // Extract item ID
						int quantity = stoi(line.substr(tabPos + 1)); // Extract quantity
						order.itemId.push_back(itemId);
						order.quantity.push_back(quantity);
					}
					catch (const exception& e) {

					}
				}
			}
		}
		file.close();

		// Validate the number of orders to show
		if (numOrders <= 0 || numOrders > static_cast<int>(orders.size())) {
			cout << "Invalid number of orders to show" << endl;
			return;
		}

		// Display the requested number of orders from the end
		cout << "Showing " << numOrders << " orders from the end:" << endl;
		for (int i = static_cast<int>(orders.size()) - numOrders; i < static_cast<int>(orders.size()); ++i) {
			orders[i].displayOrder();
		}
	}
	

	void playInterface()
	{
		HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		short enter = '1';
		short arrowKey = '1';
		short enter1 = 192;
		short enter2 = 7;
		short enter3 = 7;
		short enter4 = 7;
		short enter5 = 7;
		string coutArray[5];
		coutArray[0].erase();
		coutArray[0] += "\n\n\t\t\t\t\tCheck Inventory";
		coutArray[1].erase();
		coutArray[1] += "\n\n\t\t\t\t\tAdd Customer Order";
		coutArray[2].erase();
		coutArray[2] += "\n\n\t\t\t\t\tRefresh Inventory";
		coutArray[3].erase();
		coutArray[3] += "\n\n\t\t\t\t\tView Orders";
		coutArray[4].erase();
		coutArray[4] += "\n\n\t\t\t\t\tExit\n\n\n\n\n\n\n";
		short coutArrayIndex = 0;
		system("CLS");

		cout << "\n\n\n                                                                   POSSystem\n\n\n\n\n\n\n\t\t\t";
		SetConsoleTextAttribute(h, enter1);
		cout << coutArray[0];
		SetConsoleTextAttribute(h, enter2);
		cout << coutArray[1];
		SetConsoleTextAttribute(h, enter3);
		cout << coutArray[2];
		SetConsoleTextAttribute(h, enter4);
		cout << coutArray[3];
		SetConsoleTextAttribute(h, enter5);
		cout << coutArray[4];
		SetConsoleTextAttribute(h, 7);
		cout << endl;

		while (enter != 13)
		{
			enter = _getch();
			if (enter == 13)
			{
				break;
			}
			//Enter = 13 and \n = 10
			if (enter == 224)
			{
				arrowKey = _getch();
				if (arrowKey == 72) //up
				{
					if (coutArrayIndex == 0)
					{
						coutArrayIndex = 4;
					}
					else
					{
						coutArrayIndex--;
					}
				}
				else if (arrowKey == 80) //down
				{
					coutArrayIndex = ++coutArrayIndex % 5;
				}
				if (coutArrayIndex == 0)
				{
					enter1 = 192;
					enter2 = 7;
					enter3 = 7;
					enter4 = 7;
					enter5 = 7;
				}
				else if (coutArrayIndex == 1)
				{
					enter1 = 7;
					enter2 = 192;
					enter3 = 7;
					enter4 = 7;
					enter5 = 7;
				}
				else if (coutArrayIndex == 2)
				{
					enter1 = 7;
					enter2 = 7;
					enter3 = 192;
					enter4 = 7;
					enter5 = 7;
				}
				else if (coutArrayIndex == 3)
				{
					enter1 = 7;
					enter2 = 7;
					enter3 = 7;
					enter4 = 192;
					enter5 = 7;
				}
				else if (coutArrayIndex == 4)
				{
					enter1 = 7;
					enter2 = 7;
					enter3 = 7;
					enter4 = 7;
					enter5 = 192;
				}
				system("CLS");
				cout << "\n\n\n                                                                   POSSystem\n\n\n\n\n\n\n\t\t\t";
				SetConsoleTextAttribute(h, enter1);
				cout << coutArray[0];
				SetConsoleTextAttribute(h, enter2);
				cout << coutArray[1];
				SetConsoleTextAttribute(h, enter3);
				cout << coutArray[2];
				SetConsoleTextAttribute(h, enter4);
				cout << coutArray[3];
				SetConsoleTextAttribute(h, enter5);
				cout << coutArray[4];
				SetConsoleTextAttribute(h, 7);
				cout << endl;
			}
			else
			{
				continue;
			}
		}
		if (coutArrayIndex == 0) //Check Inventory
		{
			system("CLS");
			cout << "\n\n\n                                                                   Inventory\n\n\n\n\n\n\n";
			displayItems("Items.txt");
			_getch();

		}
		else if (coutArrayIndex == 1) //Add Customer Order
		{
			system("CLS");
			processOrder("Items.txt");
			_getch();
		}
		else if (coutArrayIndex == 2) //Refresh Inventory
		{
			system("CLS");
			char choice = 'y';
			while (choice == 'y' || choice == 'Y')
			{
				updateInventory("Items.txt");
				cout << "Update another item? Press anything except \'y\' and \'Y\' for no: ";
				cin >> choice;
			}
			_getch();
		}
		else if (coutArrayIndex == 3) //View Orders
		{
			system("CLS");
			showOrders("Orders.txt");
			_getch();
		}
		else if (coutArrayIndex == 4) //Exit
		{
			cout << "\n\n\n\n\n\n\n";
			exit(0);
		}
		playInterface();
		return;
	}
};
int main()
{
	POSSystem a;
	a.playInterface();

	_getch();
	return 0;
}
