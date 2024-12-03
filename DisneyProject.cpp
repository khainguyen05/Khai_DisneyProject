#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stack>
#include <vector>
#include <algorithm>
using namespace std;

// Struct dai dien cho du lieu Disney
struct Disney {
    string Date;
    float Open_Price;
    float High_Price;
    float Low_Price;
    float Close_Price;
    int Volume;
};

// Struct dai dien cho node trong cay nhi phan
struct TreeNode {
    Disney data;
    TreeNode* left;
    TreeNode* right;
    TreeNode(Disney x) : data(x), left(nullptr), right(nullptr) {}
};

// Ham tao doi tuong Disney tu mot chuoi du lieu
Disney CreateObject(const string& line) {
    Disney d;
    try {
        stringstream ss(line);
        string temp;

        getline(ss, d.Date, ',');
        getline(ss, temp, ','); d.Open_Price = stof(temp);
        getline(ss, temp, ','); d.High_Price = stof(temp);
        getline(ss, temp, ','); d.Low_Price = stof(temp);
        getline(ss, temp, ','); d.Close_Price = stof(temp);
        getline(ss, temp, ','); d.Volume = stoi(temp);
    }
    catch (const exception& e) {
        cout << "Loi khi phan tich dong: " << line << ". Ngoai le: " << e.what() << endl;
    }
    return d;
}

// Ham chen du lieu vao cay nhi phan tim kiem theo ngay (Date)
TreeNode* insert(TreeNode* root, Disney x) {
    if (root == nullptr) {
        return new TreeNode(x);
    }

    TreeNode* current = root;
    TreeNode* parent = nullptr;
    while (current != nullptr) {
        parent = current;
        if (x.Date < current->data.Date) {
            current = current->left;
        }
        else if (x.Date > current->data.Date) {
            current = current->right;
        }
        else {
            return root; // Ngay da ton tai
        }
    }

    if (x.Date < parent->data.Date) {
        parent->left = new TreeNode(x);
    }
    else {
        parent->right = new TreeNode(x);
    }
    return root;
}

// Ham doc du lieu tu file
void readFromFile(TreeNode*& root, const string& filename) {
    ifstream input(filename);
    if (!input.is_open()) {
        cout << "Khong the mo file " << filename << endl;
        return;
    }

    string line;
    bool isFirstLine = true; // Bo qua dong dau (header)
    while (getline(input, line)) {
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }
        if (!line.empty()) {
            try {
                Disney data = CreateObject(line);
                root = insert(root, data);
            }
            catch (const exception& e) {
                cout << "Bo qua dong khong hop le: " << line << endl;
            }
        }
    }
    input.close();
    cout << "Doc file thanh cong." << endl;
}

// Inorder Traversal voi kha nang sap xep theo tieu chi va thu tu
void inorderTraversal(TreeNode* root, int& count, const string& criteria, bool ascending) {
    if (root == nullptr || count <= 0) return;

    stack<TreeNode*> s; // Su dung stack de tranh tran bo nho
    TreeNode* current = root;
    vector<Disney> dataList; // Su dung vector de luu cac phan tu vao
    while (current != NULL || !s.empty()) {
        while (current != NULL) {
            s.push(current); // Day nut hien tai vao stack
            current = current->left; // Duyet trai
        }
        current = s.top(); // Lay nut tren cung ra
        s.pop();   // Xoa nut do khoi stack
        dataList.push_back(current->data); // Them du lieu cua nut do vao vector danh sach
        current = current->right; // Duyet phai
    }
    sort(dataList.begin(), dataList.end(), [&](const Disney& a, const Disney& b) {
        if (criteria == "Open_Price") {
            if (ascending) {
                return a.Open_Price < b.Open_Price;  // Tang dan
            }
            else {
                return a.Open_Price > b.Open_Price; // Giam dan
            }
        }
        else if (criteria == "High_Price") {
            if (ascending) {
                return a.High_Price < b.High_Price;
            }
            else {
                return a.High_Price > b.High_Price;
            }
        }
        else if (criteria == "Low_Price") {
            if (ascending) {
                return a.Low_Price < b.Low_Price;
            }
            else {
                return a.Low_Price > b.Low_Price;
            }
        }
        else if (criteria == "Close_Price") {
            if (ascending) {
                return a.Close_Price < b.Close_Price;
            }
            else {
                return a.Close_Price > b.Close_Price;
            }
        }
        else if (criteria == "Volume") {
            if (ascending) {
                return a.Volume < b.Volume;
            }
            else {
                return a.Volume > b.Volume;
            }
        }
        else {
            if (ascending) {
                return a.Date < b.Date;
            }
            else {
                return a.Date > b.Date;
            }
        }
        });

    // In ket qua sap xep
    for (int i = 0; i < count && i < dataList.size(); ++i) {
        cout << "Date: " << dataList[i].Date
            << ", Open Price: " << dataList[i].Open_Price
            << ", High Price: " << dataList[i].High_Price
            << ", Low Price: " << dataList[i].Low_Price
            << ", Close Price: " << dataList[i].Close_Price
            << ", Volume: " << dataList[i].Volume << endl;
    }
}

// Tim ngay co gia tri lon nhat theo tieu chi
Disney findMax(TreeNode* root, const string& type) {
    if (root == nullptr) {
        throw runtime_error("Cay trong!");
    }

    stack<TreeNode*> s;
    TreeNode* current = root;
    Disney maxDisney = root->data;

    while (current != nullptr || !s.empty()) {
        while (current != nullptr) {
            s.push(current);
            current = current->left;
        }

        current = s.top();
        s.pop();

        if ((type == "Open_Price" && current->data.Open_Price > maxDisney.Open_Price) ||
            (type == "High_Price" && current->data.High_Price > maxDisney.High_Price) ||
            (type == "Low_Price" && current->data.Low_Price > maxDisney.Low_Price) ||
            (type == "Close_Price" && current->data.Close_Price > maxDisney.Close_Price) ||
            (type == "Volume" && current->data.Volume > maxDisney.Volume)) {
            maxDisney = current->data;
        }

        current = current->right;
    }
    return maxDisney;
}

// Tim node co gia tri nho nhat trong cay (dung khi xoa node)
TreeNode* minNode(TreeNode* root) {
    if (root == NULL) {
        return NULL;
    }
    TreeNode* current = root;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

// Ham tach ngay, thang, nam tu chuoi "yyyy-mm-dd"
void splitDate(const std::string& dateStr, int& year, int& month, int& day)
{
    sscanf_s(dateStr.c_str(), "%d-%d-%d", &year, &month, &day);
}
//Xoa mot Node theo ngay
TreeNode* deleteNodeByDate(TreeNode* root, int year, int month, int day)
{
    if (root == NULL)
    {
        return root;
    }
    //tac nay thang nang tu node root->data.Date
    int nodeYear, nodeMonth, nodeDay;
    splitDate(root->data.Date, nodeYear, nodeMonth, nodeDay);
    if (year < nodeYear || (year == nodeYear && month < nodeMonth) || (year == nodeYear && month == nodeMonth && day < nodeDay))
    {
        //duyet ben trai
        root->left = deleteNodeByDate(root->left, year, month, day);
    }
    else if (year > nodeYear || (year == nodeYear && month > nodeMonth) || (year == nodeYear && month == nodeMonth && day > nodeDay))
    {
        //duyet ben phai
        root->right = deleteNodeByDate(root->right, year, month, day);
    }
    else {
        //Neu node can xoa 
        if (root->left == NULL and root->right == NULL)//Khong co con
        {
            delete root;
            return NULL;
        }
        else if (root->left == NULL) //cay co con phai
        {
            TreeNode* tmp = root->right; //luu con phai vao node tam
            delete root;
            return tmp;
        }
        else if (root->right == NULL)// chi co con trai
        {
            TreeNode* tmp = root->left; //luu con trai vao node tam
            delete root;
            return tmp;
        }
        else// neu cay co ca con trai va con phai

        {
            TreeNode* temp = minNode(root->right); //tim node nho nhat ben phai de luu vao node tam
            int tempYear, tempMonth, tempDay;
            splitDate(temp->data.Date, tempYear, tempMonth, tempDay);
            root->data = temp->data;//gan du lieu cua node nho nhat vao node can xoa
            root->right = deleteNodeByDate(root->right, tempYear, tempMonth, tempDay);// xoa node nho nhat o cay con phai
        }
    }
    return root;
}
//xuat thong tin theo nam 
void printInfoByYear(TreeNode* root, int year) {
    if (root == nullptr) return;

    stack<TreeNode*> s;
    TreeNode* current = root;
    vector<Disney> dataList;

    //duyet cay su dung stack 
    while (current != nullptr || !s.empty()) {
        while (current != nullptr) {
            s.push(current);
            current = current->left;
        }
        current = s.top();
        s.pop();

       
        int nodeYear, nodeMonth, nodeDay;
        splitDate(current->data.Date, nodeYear, nodeMonth, nodeDay);

        //Kiem tra nam co hop le hay khong 
        if (nodeYear == year) {
            dataList.push_back(current->data);
        }

        current = current->right;
    }

    
    if (dataList.empty()) {
        cout << "Khong co du lieu trong nam " << year << "." << endl;
    }
    else {
        cout << "Thong tin trong nam " << year << ":\n";
        for (const auto& data : dataList) {
            cout << "Date: " << data.Date
                << ", Open Price: " << data.Open_Price
                << ", High Price: " << data.High_Price
                << ", Low Price: " << data.Low_Price
                << ", Close Price: " << data.Close_Price
                << ", Volume: " << data.Volume << endl;
        }
    }
}

// Ham ghi du lieu vao file
void writeFile(TreeNode* root, const string& filename) {
    ofstream output(filename);
    if (!output.is_open()) {
        cout << "Khong the mo file de ghi" << endl;
        return;
    }

    stack<TreeNode*> s;
    TreeNode* current = root;
    while (current != nullptr || !s.empty()) {
        while (current != nullptr) {
            s.push(current);
            current = current->left;
        }
        current = s.top();
        s.pop();
        output << current->data.Date << ","
            << current->data.Open_Price << ","
            << current->data.High_Price << ","
            << current->data.Low_Price << ","
            << current->data.Close_Price << ","
            << current->data.Volume << "\n";
        current = current->right;
    }

    output.close();
    cout << "Du lieu da duoc ghi vao file." << endl;
}
void displayMenu() {
    cout << "\n====== MENU ======\n";
    cout << "1. Xuat thong tin theo thu tu tang dan (Inorder Traversal)\n";
    cout << "2. Tim ngay cao nhat theo tieu chi (Open_price,Closed_price, Volume)\n";
    cout << "3. Chinh sua cay: Them thong tin vao cay\n";
    cout << "4. Chinh sua cay: Xoa du lieu theo (yyyy-mm-dd)\n";
    cout << "5. Xuat thong tin theo nam\n";
    cout << "6. Thoat\n";
    cout << "==================\n";
    cout << "Nhap lua chon cua ban: ";
}

int main() {
    TreeNode* root = nullptr;
    readFromFile(root, "C:/DSA/DisneyList.TXT");

    if (root == nullptr) {
        cout << "The tree is empty." << endl;
        return 0;
    }

    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1: {
            int count;
            cout << "Nhap vao so luong muon xuat thong tin: ";
            cin >> count;
            int choice2;
            cout << "Nhap lua chon tieu chi (1. Open_Price, 2. High_Price, 3. Low_Price, 4. Close_Price, 5. Volume, 6. Date): ";
            cin >> choice2;
            string criteria;
            switch(choice2)
            {
                case 1: criteria = "Open_Price"; break;
                case 2: criteria = "High_Price"; break;
                case 3: criteria = "Low_Price"; break;
                case 4: criteria = "Close_Price"; break;
                case 5: criteria = "Volume"; break;
                case 6: criteria = "Date"; break;
                default: cout << "Lua chon khong hop le!" << endl; continue;
            }
            bool ascending;
            cout << "Chon thu tu mong muon (1.Tang dan , 0. Giam dan): ";
            cin >> ascending;
            inorderTraversal(root, count, criteria, ascending);
            break;
        }
        case 2: {
            int subChoice;
            cout << "Nhap lua chon de in ra theo tieu chi (1.Open_Price, 2. Close_Price, 3.Volume): ";
            cin >> subChoice;
            switch (subChoice)
            {
            case 1: {
                Disney maxOpen = findMax(root, "Open_Price");
                cout << "Ngay co Open Price cao nhat:\n";
                cout << "Date: " << maxOpen.Date
                    << ", Open Price: " << maxOpen.Open_Price << endl;
                break;
            }
            case 2: {
                Disney maxClose = findMax(root, "Close_Price");
                cout << "Ngay co Close Price cao nhat:\n";
                cout << "Date: " << maxClose.Date
                    << ", Close Price: " << maxClose.Close_Price << endl;
                break;
            }
            case 3: {
                Disney maxVolume = findMax(root, "Volume");
                cout << "Ngay co Volume lon nhat:\n";
                cout << "Date: " << maxVolume.Date
                    << ", Volume: " << maxVolume.Volume << endl;
                break;
            }
            }
            break;
        }
        case 3: {
            string date;
            float open, high, low, close;
            int volume;

            cout << "Nhap ngay (yyyy-mm-dd): ";
            cin >> date;
            cout << "Nhap gia Open Price: ";
            cin >> open;
            cout << "Nhap gia High Price: ";
            cin >> high;
            cout << "Nhap gia Low Price: ";
            cin >> low;
            cout << "Nhap gia Close Price: ";
            cin >> close;
            cout << "Nhap luong (Volume): ";
            cin >> volume;
            Disney newDisney = { date, open, high, low, close, volume };
            root = insert(root, newDisney);
            writeFile(root, "C:/DSA/DisneyList.TXT");
            cout << "Thong tin da duoc them vao cay va ghi vao file";
            break;
        }
        case 4:
        {
            int year, month, day;
            cout << "Nhap nam: ";
            cin >> year;
            cout << "Nhap thang: ";
            cin >> month;
            cout << "Nhap ngay: ";
            cin >> day;
            root = deleteNodeByDate(root, year, month, day);
            writeFile(root, "C:/DSA/DisneyList.TXT");
            cout << "Node voi ngay " << year << "-" << month << "-" << day << "da duoc xoa va ghi lai vao file.\n";
            break;

        }
        case 5:
        {
            int year;
            cout << "Nhap nam ban muon xem thong tin: ";
            cin >> year;
            printInfoByYear(root, year);
            break;
        }
        case 6:
            cout << "Thoat chuong trinh." << endl;
            break;
        default:
            cout << "Lua chon khong hop le. Vui long thu lai." << endl;
        }
    } while (choice != 6);

    return 0;
}

