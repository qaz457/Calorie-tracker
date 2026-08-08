#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include <conio.h>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

class EscException : public exception {};

string inputStringEsc() {
    string input = "";
    while (true) {
        char ch = _getch();
        if (ch == 27) {
            cout << endl;
            throw EscException();
        }
        if (ch == 13) {
            cout << endl;
            break;
        }
        if (ch == 8) {
            if (!input.empty()) {
                input.pop_back();
                cout << "\b \b";
            }
        }
        else if (isprint((unsigned char)ch) || (ch >= -64 && ch <= -1)) {
            input += ch;
            cout << ch;
        }
    }
    return input;
}

double inputDoubleEsc() {
    string s = inputStringEsc();
    if (s.empty()) return 0;
    try {
        return stod(s);
    }
    catch (...) {
        return -1;
    }
}

int inputIntEsc() {
    string s = inputStringEsc();
    if (s.empty()) return 0;
    try {
        return stoi(s);
    }
    catch (...) {
        return -1;
    }
}

unsigned char toLowerChar(unsigned char ch) {
    if (ch >= 0xC0 && ch <= 0xDF) {
        return ch + 0x20;
    }
    if (ch == 0xA8) {
        return 0xB8;
    }
    return (unsigned char)::tolower(ch);
}

string toLower(const string& s) {
    string result = s;
    for (char& c : result) {
        c = (char)toLowerChar((unsigned char)c);
    }
    return result;
}

void getDateNow(int& day, int& month, int& year) {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    if (now) {
        day = now->tm_mday;
        month = now->tm_mon + 1;
        year = now->tm_year + 1900;
    }
    else {
        day = 1; month = 1; year = 2026;
    }
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#endif
}

void pause() {
    cout << "Нажмите Enter чтобы продолжить...";
    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
    cin.get();
}

enum Gender { Male, Female };
enum ActivityLevel { Sedentary, Light, Moderate, Active, VeryActive };

class Validator {
public:
    static bool isValidLogin(const string& login) {
        if (login.empty()) return false;
        if (login.length() < 3) return false;
        if (login.length() > 20) return false;

        for (char c : login) {
            if (!isalnum((unsigned char)c) && c != '_' && c != '-') {
                return false;
            }
        }
        return true;
    }

    static bool isValidPassword(const string& password) {
        if (password.empty()) return false;
        if (password.length() < 3) return false;
        if (password.length() > 30) return false;
        return true;
    }

    static bool isValidHeight(int height) {
        return height >= 50 && height <= 300;
    }

    static bool isValidWeight(double weight) {
        return weight >= 10 && weight <= 500;
    }

    static bool isValidAge(int age) {
        return age >= 1 && age <= 120;
    }

    static bool isValidGender(int gender) {
        return gender == 0 || gender == 1;
    }

    static bool isValidActivity(int activity) {
        return activity >= 0 && activity <= 4;
    }

    static bool isValidProductName(const string& name) {
        if (name.empty()) return false;
        if (name.length() < 2) return false;
        return true;
    }

    static bool isValidPortionWeight(double weight) {
        return weight > 0 && weight <= 10000;
    }

    static bool isValidProductIndex(int index, size_t maxSize) {
        return index > 0 && index <= (int)maxSize;
    }
};

class User {
private:
    string login;
    string password;
    Gender gender = Male;
    int height = 0;
    double weight = 0.0;
    int age = 0;
    ActivityLevel level = Sedentary;

    const double getActivityFactor() {
        switch (level) {
        case Sedentary:  return 1.2;
        case Light:      return 1.375;
        case Moderate:   return 1.55;
        case Active:     return 1.725;
        case VeryActive: return 1.9;
        default:         return 1.2;
        }
    }

public:
    User() = default;

    User(const string& l, const string& p, Gender g, int h, double w, int a, ActivityLevel al)
        : login(l), password(p), gender(g), height(h), weight(w), age(a), level(al) {
    }

    double CalculateTDEE() {
        if (gender == Male) {
            return (10 * weight + 6.25 * height - 5 * age + 5) * getActivityFactor();
        }
        else {
            return (10 * weight + 6.25 * height - 5 * age - 161) * getActivityFactor();
        }
    }

    void getPFCnorm(double& protein, double& fat, double& carbs) {
        double TDEE = CalculateTDEE();
        protein = (TDEE * 0.3) / 4.0;
        fat = (TDEE * 0.25) / 9.0;
        carbs = (TDEE * 0.45) / 4.0;
    }

    string ToFileStr() {
        return login + ";" + password + ";" + to_string(gender) + ";"
            + to_string(height) + ";" + to_string(weight) + ";" + to_string(age) + ";"
            + to_string(level);
    }

    string getLogin() const { return login; }
    string getPassword() const { return password; }
    Gender getGender() const { return gender; }
    int getHeight() const { return height; }
    double getWeight() const { return weight; }
    int getAge() const { return age; }
    ActivityLevel getActivity() const { return level; }

    void setPassword(const string& p) { password = p; }
    void setGender(Gender g) { gender = g; }
    void setHeight(int h) { height = h; }
    void setWeight(double w) { weight = w; }
    void setAge(int a) { age = a; }
    void setActivity(ActivityLevel al) { level = al; }
};

class Product {
private:
    string name;
    double calories = 0;
    double protein = 0;
    double fat = 0;
    double carbs = 0;

public:
    Product() = default;

    Product(string n, double kcal, double p, double f, double c)
        : name(n), calories(kcal), protein(p), fat(f), carbs(c) {
    }

    string getName() const { return name; }
    double getCalories() const { return calories; }
    double getProtein() const { return protein; }
    double getFat() const { return fat; }
    double getCarbs() const { return carbs; }

    static bool validator(double kcal, double p, double f, double c) {
        if (kcal < 0 || p < 0 || f < 0 || c < 0) return false;

        if (kcal > 1000 || p > 100 || f > 100 || c > 100) {
            cout << "Слишком большие значения! Проверьте ввод." << endl;
            return false;
        }

        double kcal_calc = f * 9 + p * 4 + c * 4;
        if (kcal_calc == 0) {
            cout << "Сумма БЖУ не может быть равна 0!" << endl;
            return false;
        }

        double diff = abs(kcal - kcal_calc) / kcal_calc;
        if (diff > 0.05) {
            cout << "Введенный калораж не совпадает с КБЖУ (разница > 5%)" << endl;
            return false;
        }
        if (p > 100 || f > 100 || c > 100) return false;
        return true;
    }

    static Product InputProduct() {
        string name;
        double kcal, p, f, c;

        cout << "Введите название продукта: ";
        name = inputStringEsc();

        while (!Validator::isValidProductName(name)) {
            cout << "Некорректное название! Введите минимум 2 символа: ";
            name = inputStringEsc();
        }

        do {
            cout << "Калории на 100 г: ";  kcal = inputDoubleEsc();
            cout << "Белки (г на 100 г): "; p = inputDoubleEsc();
            cout << "Жиры (г на 100 г): ";  f = inputDoubleEsc();
            cout << "Углеводы (г на 100 г): "; c = inputDoubleEsc();

            if (!validator(kcal, p, f, c)) {
                cout << "Данные некорректны. Повторите ввод." << endl;
            }
            else {
                break;
            }
        } while (true);
        return Product(name, kcal, p, f, c);
    }

    string toFileStr() const {
        return name + ";" + to_string(calories) + ";" + to_string(protein) + ";"
            + to_string(fat) + ";" + to_string(carbs);
    }

    static Product fromFileStr(const string& line) {
        stringstream ss(line);
        string name, k_s, p_s, f_s, c_s;

        getline(ss, name, ';');
        getline(ss, k_s, ';');
        getline(ss, p_s, ';');
        getline(ss, f_s, ';');
        getline(ss, c_s);

        return Product(name, stod(k_s), stod(p_s), stod(f_s), stod(c_s));
    }
};

class ProductDB {
private:
    vector<Product> products;
    const string filename = "products.txt";

    void loadFromFile() {
        ifstream file(filename);
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                products.push_back(Product::fromFileStr(line));
            }
        }
        file.close();
    }

    void saveToFile() const {
        ofstream file(filename);
        for (const auto& p : products) {
            file << p.toFileStr() << endl;
        }
        file.close();
    }

public:
    ProductDB() {
        loadFromFile();
        if (products.empty()) {
            products = {
                {"Куриная грудка", 165, 31.0, 3.6, 0.0},
                {"Рис отварной", 130, 2.7, 0.3, 28.0},
                {"Яблоко", 52, 0.3, 0.2, 14.0},
                {"Творог 5%", 121, 17.0, 5.0, 2.0}
            };
            saveToFile();
        }
    }

    void addProduct(const Product& p) {
        products.push_back(p);
        saveToFile();
    }

    vector<Product> search(const string& query) const {
        vector<Product> result;
        string q = toLower(query);
        for (const auto& p : products) {
            if (toLower(p.getName()).find(q) != string::npos) {
                result.push_back(p);
            }
        }
        return result;
    }

    void showAll() const {
        if (products.empty()) {
            cout << "База данных пуста" << endl;
            return;
        }

        cout << endl << "=== СПИСОК ДОСТУПНЫХ ПРОДУКТОВ ===" << endl
            << left << setw(25) << "Название" << setw(12) << "ккал/100г"
            << setw(10) << "Белки" << setw(10) << "Жиры" << setw(10) << "Углеводы" << endl
            << string(67, '-') << endl;

        for (const auto& p : products) {
            cout << left << setw(25) << p.getName() << fixed << setprecision(1)
                << setw(12) << p.getCalories() << setw(10) << p.getProtein()
                << setw(10) << p.getFat() << setw(10) << p.getCarbs() << endl;
        }
        cout << defaultfloat;
    }

    void sortProducts(int sortField, bool ascending) {
        sort(products.begin(), products.end(), [sortField](const Product& a, const Product& b) {
            switch (sortField) {
            case 1: return toLower(a.getName()) < toLower(b.getName());
            case 2: return a.getCalories() < b.getCalories();
            case 3: return a.getProtein() < b.getProtein();
            case 4: return a.getFat() < b.getFat();
            case 5: return a.getCarbs() < b.getCarbs();
            default: return toLower(a.getName()) < toLower(b.getName());
            }
            });

        if (!ascending) {
            reverse(products.begin(), products.end());
        }

        saveToFile();
    }

    void sortMenu() {
        if (products.empty()) {
            cout << "База данных пуста, сортировать нечего." << endl;
            return;
        }

        cout << endl << "=== СОРТИРОВКА СПИСКА ПРОДУКТОВ ===" << endl;
        cout << "По какому параметру сортировать?" << endl;
        cout << "1 - По названию" << endl;
        cout << "2 - По калорийности" << endl;
        cout << "3 - По белкам" << endl;
        cout << "4 - По жирам" << endl;
        cout << "5 - По углеводам" << endl;
        cout << "Ваш выбор: ";

        int field = inputIntEsc();
        if (field < 1 || field > 5) {
            cout << "Некорректный выбор. Сортировка отменена." << endl;
            return;
        }

        cout << "Порядок сортировки:" << endl;
        cout << "1 - По возрастанию" << endl;
        cout << "2 - По убыванию" << endl;
        cout << "Ваш выбор: ";

        int order = inputIntEsc();
        if (order != 1 && order != 2) {
            cout << "Некорректный выбор. Сортировка отменена." << endl;
            return;
        }

        sortProducts(field, order == 1);
        cout << "Список продуктов отсортирован!" << endl;
        showAll();
    }
};

class FoodCalculator {
private:
    string name;
    double weight = 0;
    double calories = 0;
    double protein = 0;
    double fat = 0;
    double carbs = 0;
    time_t timeeat = 0;

public:
    FoodCalculator() = default;

    FoodCalculator(const Product& p, double g, time_t t = time(nullptr)) {
        name = p.getName();
        weight = g;
        calories = p.getCalories() * g / 100.0;
        protein = p.getProtein() * g / 100.0;
        fat = p.getFat() * g / 100.0;
        carbs = p.getCarbs() * g / 100.0;
        timeeat = t;
    }

    string getName() const { return name; }
    double getCalories() const { return calories; }
    double getProtein() const { return protein; }
    double getFat() const { return fat; }
    double getCarbs() const { return carbs; }

    string toFileStr() const {
        return name + ";" + to_string(weight) + ";" + to_string(calories) + ";" +
            to_string(protein) + ";" + to_string(fat) + ";" + to_string(carbs) + ";" +
            to_string(timeeat);
    }

    static FoodCalculator fromFileStr(const string& line) {
        stringstream ss(line);
        FoodCalculator m;
        string w, kcal, p, f, c, t;

        getline(ss, m.name, ';');
        getline(ss, w, ';');        m.weight = stod(w);
        getline(ss, kcal, ';');     m.calories = stod(kcal);
        getline(ss, p, ';');        m.protein = stod(p);
        getline(ss, f, ';');        m.fat = stod(f);
        getline(ss, c, ';');        m.carbs = stod(c);
        getline(ss, t);             m.timeeat = stoll(t);

        return m;
    }
};

class FoodStatistic {
private:
    vector<FoodCalculator> meals;
    string filename;

    void loadFromFile() {
        meals.clear();
        ifstream file(filename);
        if (!file.is_open()) return;
        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                meals.push_back(FoodCalculator::fromFileStr(line));
            }
        }
        file.close();
    }

    void saveToFile() const {
        ofstream file(filename);
        for (const auto& m : meals) {
            file << m.toFileStr() << endl;
        }
        file.close();
    }

public:
    FoodStatistic(const string& userLogin) {
        filename = userLogin + "_diary.txt";
        loadFromFile();
    }

    void addMeal(const FoodCalculator& m) {
        meals.push_back(m);
        saveToFile();
    }

    double totalCalories() const {
        double result = 0;
        for (const auto& m : meals) { result += m.getCalories(); }
        return result;
    }

    double totalProtein() const {
        double result = 0;
        for (const auto& m : meals) { result += m.getProtein(); }
        return result;
    }

    double totalFat() const {
        double result = 0;
        for (const auto& m : meals) { result += m.getFat(); }
        return result;
    }

    double totalCarbs() const {
        double result = 0;
        for (const auto& m : meals) { result += m.getCarbs(); }
        return result;
    }

    void closeDay() {
        meals.clear();
        saveToFile();
    }

    void showReport(User& user) {
        double kcal = totalCalories();
        double tdee = user.CalculateTDEE();
        double normP, normF, normC;
        user.getPFCnorm(normP, normF, normC);

        cout << endl << "================== ОТЧЁТ ЗА СЕГОДНЯ ==================" << endl;
        cout << "Калории: " << fixed << setprecision(0) << kcal << " / " << tdee << " ккал (";
        if (kcal <= tdee) {
            cout << "осталось " << tdee - kcal << " ккал";
        }
        else {
            cout << "перебор " << kcal - tdee << " ккал";
        }
        cout << ")" << endl << setprecision(1);

        cout << "Белки:    " << totalProtein() << " / " << normP << " г (" << (normP ? (totalProtein() / normP * 100) : 0) << "%)" << endl;
        cout << "Жиры:     " << totalFat() << " / " << normF << " г (" << (normF ? (totalFat() / normF * 100) : 0) << "%)" << endl;
        cout << "Углеводы: " << totalCarbs() << " / " << normC << " г (" << (normC ? (totalCarbs() / normC * 100) : 0) << "%)" << endl;
        cout << "=======================================================" << endl;
        cout << defaultfloat;
    }

    void getDailyStatistic(double& calories, double& protein, double& fat, double& carbs) const {
        calories = totalCalories();
        protein = totalProtein();
        fat = totalFat();
        carbs = totalCarbs();
    }
};

struct DailyStatistic {
    int day, month, year;
    double calories, protein, fat, carbs;
};

class History {
private:
    vector<DailyStatistic> history;
    string filename;

    void loadFromFile() {
        history.clear();
        ifstream file(filename);
        if (!file.is_open()) return;

        string line;
        getline(file, line);

        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            DailyStatistic ds;
            string d, m, y, kcal, p, f, c;

            getline(ss, d, ';');   ds.day = stoi(d);
            getline(ss, m, ';');   ds.month = stoi(m);
            getline(ss, y, ';');   ds.year = stoi(y);
            getline(ss, kcal, ';'); ds.calories = stod(kcal);
            getline(ss, p, ';');   ds.protein = stod(p);
            getline(ss, f, ';');   ds.fat = stod(f);
            getline(ss, c);        ds.carbs = stod(c);

            history.push_back(ds);
        }
        file.close();
    }

    void saveToFile() const {
        ofstream file(filename);
        file << "day;month;year;calories;protein;fat;carbs" << endl;
        for (const auto& ds : history) {
            file << ds.day << ";" << ds.month << ";" << ds.year << ";"
                << ds.calories << ";" << ds.protein << ";" << ds.fat << ";" << ds.carbs << endl;
        }
        file.close();
    }

public:
    History(const string& userLogin) {
        filename = userLogin + "_history.txt";
        loadFromFile();
    }

    void addTodayStat(const FoodStatistic& fs) {
        int day, month, year;
        getDateNow(day, month, year);

        double kcal, p, f, c;
        fs.getDailyStatistic(kcal, p, f, c);

        for (auto& ds : history) {
            if (ds.day == day && ds.month == month && ds.year == year) {
                ds.calories = kcal;
                ds.protein = p;
                ds.fat = f;
                ds.carbs = c;
                saveToFile();
                return;
            }
        }

        history.push_back({ day, month, year, kcal, p, f, c });
        saveToFile();
    }

    void showHistory() const {
        if (history.empty()) {
            cout << "История пуста" << endl;
            return;
        }
        cout << endl << "================== ИСТОРИЯ ПИТАНИЯ ==================" << endl;
        cout << left << setw(12) << "Дата" << setw(12) << "Калории"
            << setw(12) << "Белки" << setw(12) << "Жиры" << setw(12) << "Углеводы" << endl
            << string(60, '-') << endl;
        for (const auto& ds : history) {
            cout << left << setw(12) << (to_string(ds.day) + "." + to_string(ds.month) + "." + to_string(ds.year))
                << fixed << setprecision(0) << setw(12) << ds.calories
                << setprecision(1) << setw(12) << ds.protein
                << setw(12) << ds.fat
                << setw(12) << ds.carbs
                << endl;
        }
        cout << "=======================================================" << endl;
        cout << defaultfloat;
    }
};

class UserStatistic {
private:
    vector<User> users;
    const string filename = "users.txt";

    void loadFromFile() {
        users.clear();
        ifstream file(filename);
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string login, password, g, h, w, a, al;
            getline(ss, login, ';');
            getline(ss, password, ';');
            getline(ss, g, ';');
            getline(ss, h, ';');
            getline(ss, w, ';');
            getline(ss, a, ';');
            getline(ss, al);

            Gender gender = (Gender)stoi(g);
            int height = stoi(h);
            double weight = stod(w);
            int age = stoi(a);
            ActivityLevel activityType = (ActivityLevel)stoi(al);

            users.push_back(User(login, password, gender, height, weight, age, activityType));
        }
        file.close();
    }

    void saveToFile() const {
        ofstream file(filename);
        for (auto u : users) {
            file << u.ToFileStr() << endl;
        }
        file.close();
    }

public:
    UserStatistic() {
        loadFromFile();
    }

    bool loginAvailable(const string& login) const {
        for (const auto& u : users) {
            if (toLower(u.getLogin()) == toLower(login)) return false;
        }
        return true;
    }

    void registerUser() {
        string login, password;
        int gender, height, age, activity;
        double weight;

        cout << "=== РЕГИСТРАЦИЯ (ESC для отмены) ===" << endl;

        do {
            cout << "Введите логин (3-20 символов, буквы/цифры/_-): ";
            login = inputStringEsc();

            if (!Validator::isValidLogin(login)) {
                cout << "Некорректный логин! Используйте 3-20 символов (буквы, цифры, _, -)" << endl;
                continue;
            }

            if (!loginAvailable(login)) {
                cout << "Логин уже занят. Попробуйте другой." << endl;
            }
            else {
                break;
            }
        } while (true);

        do {
            cout << "Введите пароль (минимум 3 символа): ";
            password = inputStringEsc();
            if (!Validator::isValidPassword(password)) {
                cout << "Пароль должен содержать минимум 3 символа!" << endl;
            }
            else {
                break;
            }
        } while (true);

        do {
            cout << "Выберите пол (0 - Мужской, 1 - Женский): ";
            gender = inputIntEsc();
            if (!Validator::isValidGender(gender)) {
                cout << "Введите 0 или 1!" << endl;
            }
            else {
                break;
            }
        } while (true);

        Gender g = (gender == 0) ? Male : Female;

        do {
            cout << "Введите рост (см, 50-300): ";
            height = inputIntEsc();
            if (!Validator::isValidHeight(height)) {
                cout << "Рост должен быть от 50 до 300 см!" << endl;
            }
            else {
                break;
            }
        } while (true);

        do {
            cout << "Введите вес (кг, 10-500): ";
            weight = inputDoubleEsc();
            if (!Validator::isValidWeight(weight)) {
                cout << "Вес должен быть от 10 до 500 кг!" << endl;
            }
            else {
                break;
            }
        } while (true);

        do {
            cout << "Введите возраст (1-120): ";
            age = inputIntEsc();
            if (!Validator::isValidAge(age)) {
                cout << "Возраст должен быть от 1 до 120 лет!" << endl;
            }
            else {
                break;
            }
        } while (true);

        cout << "Выберите уровень активности:" << endl
            << "0 - Сидячий образ жизни" << endl
            << "1 - Лёгкая активность (1-3 дня в неделю)" << endl
            << "2 - Умеренная активность (3-5 дней в неделю)" << endl
            << "3 - Активный образ жизни (6-7 дней в неделю)" << endl
            << "4 - Очень активный образ жизни (тяжёлые тренировки каждый день)" << endl;

        do {
            cout << "Ваш выбор (0-4): ";
            activity = inputIntEsc();
            if (!Validator::isValidActivity(activity)) {
                cout << "Введите число от 0 до 4!" << endl;
            }
            else {
                break;
            }
        } while (true);

        ActivityLevel al = (ActivityLevel)activity;
        users.push_back(User(login, password, g, height, weight, age, al));
        saveToFile();
        cout << "Регистрация успешна!" << endl;
    }

    bool loginUser(User& loggedUser) {
        string login, password;
        cout << "=== ВХОД (ESC для отмены) ===" << endl;
        cout << "Введите логин: ";  login = inputStringEsc();
        cout << "Введите пароль: "; password = inputStringEsc();

        for (const auto& u : users) {
            if (toLower(u.getLogin()) == toLower(login) && u.getPassword() == password) {
                loggedUser = u;
                cout << "Вход успешен!" << endl;
                return true;
            }
        }
        cout << "Неверный логин или пароль." << endl;
        return false;
    }

    void editProfile(User& activeUser) {
        const char* activityNames[] = {
            "Сидячий образ жизни",
            "Лёгкая активность",
            "Умеренная активность",
            "Активный образ жизни",
            "Очень активный образ жизни"
        };

        cout << "=== РЕДАКТИРОВАНИЕ ПРОФИЛЯ (ESC для отмены) ===" << endl;
        cout << "Логин: " << activeUser.getLogin() << endl;
        cout << "Пол: " << (activeUser.getGender() == Male ? "Мужской" : "Женский") << endl;
        cout << "Рост: " << activeUser.getHeight() << " см" << endl;
        cout << "Вес: " << fixed << setprecision(1) << activeUser.getWeight() << defaultfloat << " кг" << endl;
        cout << "Возраст: " << activeUser.getAge() << " лет" << endl;
        cout << "Уровень активности: " << activityNames[activeUser.getActivity()] << endl << endl;

        cout << "Что вы хотите изменить?" << endl;
        cout << "1 - Пароль" << endl;
        cout << "2 - Пол" << endl;
        cout << "3 - Рост" << endl;
        cout << "4 - Вес" << endl;
        cout << "5 - Возраст" << endl;
        cout << "6 - Уровень активности" << endl;
        cout << "0 - Отмена" << endl;
        cout << "Ваш выбор: ";

        int choice = inputIntEsc();

        switch (choice) {
        case 1: {
            string password;
            do {
                cout << "Введите новый пароль (минимум 3 символа): ";
                password = inputStringEsc();
                if (!Validator::isValidPassword(password)) {
                    cout << "Пароль должен содержать минимум 3 символа!" << endl;
                }
                else {
                    break;
                }
            } while (true);
            activeUser.setPassword(password);
            cout << "Пароль успешно изменен!" << endl;
            break;
        }
        case 2: {
            int gender;
            do {
                cout << "Выберите пол (0 - Мужской, 1 - Женский): ";
                gender = inputIntEsc();
                if (!Validator::isValidGender(gender)) {
                    cout << "Введите 0 или 1!" << endl;
                }
                else {
                    break;
                }
            } while (true);
            activeUser.setGender(gender == 0 ? Male : Female);
            cout << "Пол успешно изменен!" << endl;
            break;
        }
        case 3: {
            int height;
            do {
                cout << "Введите рост (см, 50-300): ";
                height = inputIntEsc();
                if (!Validator::isValidHeight(height)) {
                    cout << "Рост должен быть от 50 до 300 см!" << endl;
                }
                else {
                    break;
                }
            } while (true);
            activeUser.setHeight(height);
            cout << "Рост успешно изменен!" << endl;
            break;
        }
        case 4: {
            double weight;
            do {
                cout << "Введите вес (кг, 10-500): ";
                weight = inputDoubleEsc();
                if (!Validator::isValidWeight(weight)) {
                    cout << "Вес должен быть от 10 до 500 кг!" << endl;
                }
                else {
                    break;
                }
            } while (true);
            activeUser.setWeight(weight);
            cout << "Вес успешно изменен!" << endl;
            break;
        }
        case 5: {
            int age;
            do {
                cout << "Введите возраст (1-120): ";
                age = inputIntEsc();
                if (!Validator::isValidAge(age)) {
                    cout << "Возраст должен быть от 1 до 120 лет!" << endl;
                }
                else {
                    break;
                }
            } while (true);
            activeUser.setAge(age);
            cout << "Возраст успешно изменен!" << endl;
            break;
        }
        case 6: {
            int activity;
            cout << "Выберите уровень активности:" << endl
                << "0 - Сидячий образ жизни" << endl
                << "1 - Лёгкая активность (1-3 дня в неделю)" << endl
                << "2 - Умеренная активность (3-5 дней в неделю)" << endl
                << "3 - Активный образ жизни (6-7 дней в неделю)" << endl
                << "4 - Очень активный образ жизни (тяжёлые тренировки каждый день)" << endl;
            do {
                cout << "Ваш выбор (0-4): ";
                activity = inputIntEsc();
                if (!Validator::isValidActivity(activity)) {
                    cout << "Введите число от 0 до 4!" << endl;
                }
                else {
                    break;
                }
            } while (true);
            activeUser.setActivity((ActivityLevel)activity);
            cout << "Уровень активности успешно изменен!" << endl;
            break;
        }
        case 0:
            cout << "Редактирование отменено." << endl;
            return;
        default:
            cout << "Некорректный выбор." << endl;
            return;
        }

        for (auto& u : users) {
            if (toLower(u.getLogin()) == toLower(activeUser.getLogin())) {
                u = activeUser;
                break;
            }
        }
        saveToFile();
    }
};

int main() {
#ifdef _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#else
    setlocale(LC_ALL, "ru");
#endif

    UserStatistic UserStat;
    ProductDB ProductDB;
    User activeUser;

    bool running = true;

    while (running) {
        try {
            clearScreen();
            cout << "===== МЕНЮ АВТОРИЗАЦИИ =====" << endl;
            cout << "1. Войти под своим логином" << endl;
            cout << "2. Зарегистрировать новый аккаунт" << endl;
            cout << "3. Завершить работу (или нажмите ESC)" << endl;
            cout << "============================" << endl << endl;
            cout << "Выберите действие: ";

            char ch = _getch();
            if (ch == 27 || ch == '3') {
                running = false;
                cout << endl << "До свидания!" << endl;
                break;
            }

            if (ch == '1') {
                cout << "1" << endl;
                if (UserStat.loginUser(activeUser)) {
                    FoodStatistic FoodStat(activeUser.getLogin());
                    History userHistory(activeUser.getLogin());
                    bool loggedIn = true;

                    pause();

                    while (loggedIn) {
                        try {
                            clearScreen();
                            cout << "===== ЛИЧНЫЙ КАБИНЕТ (" << activeUser.getLogin() << ") =====" << endl;
                            cout << "1. Просмотреть КБЖУ-отчет за сегодня" << endl;
                            cout << "2. Добавить прием пищи (выбрать продукт)" << endl;
                            cout << "3. Просмотреть архив (историю за прошлые дни)" << endl;
                            cout << "4. Добавить новый продукт в общую базу" << endl;
                            cout << "5. Закрыть день (сохранить итоги и очистить дневник)" << endl;
                            cout << "6. Отсортировать список продуктов" << endl;
                            cout << "7. Изменить профиль" << endl;
                            cout << "8. Выйти из аккаунта (или нажмите ESC)" << endl;
                            cout << "===============================================" << endl;
                            cout << "Выберите пункт: ";

                            char accCh = _getch();
                            if (accCh == 27 || accCh == '8') {
                                loggedIn = false;
                                cout << endl << "Выход из аккаунта..." << endl;
                                continue;
                            }

                            if (accCh == '1') {
                                cout << "1" << endl;
                                clearScreen();
                                FoodStat.showReport(activeUser);
                                pause();
                            }
                            else if (accCh == '2') {
                                cout << "2" << endl;
                                clearScreen();
                                ProductDB.showAll();
                                cout << endl << "Введите поисковый запрос (или ESC для отмены): ";
                                string searchQuery = inputStringEsc();

                                vector<Product> matches = ProductDB.search(searchQuery);
                                if (matches.empty()) {
                                    cout << "Ничего не найдено по запросу \"" << searchQuery << "\"" << endl;
                                }
                                else {
                                    cout << endl << "Результаты поиска:" << endl;
                                    for (size_t i = 0; i < matches.size(); ++i) {
                                        cout << i + 1 << ". " << matches[i].getName()
                                            << " [" << fixed << setprecision(0) << matches[i].getCalories() << " ккал/100г]" << endl;
                                    }
                                    cout << defaultfloat;

                                    cout << "Выберите номер продукта: ";
                                    int itemIndex = inputIntEsc();

                                    if (!Validator::isValidProductIndex(itemIndex, matches.size())) {
                                        cout << "Некорректный индекс." << endl;
                                        pause();
                                        continue;
                                    }

                                    cout << "Укажите массу порции (в граммах, 1-10000): ";
                                    double portionWeight = inputDoubleEsc();

                                    if (!Validator::isValidPortionWeight(portionWeight)) {
                                        cout << "Некорректный вес порции!" << endl;
                                        pause();
                                        continue;
                                    }

                                    FoodCalculator entry(matches[itemIndex - 1], portionWeight);
                                    FoodStat.addMeal(entry);
                                    userHistory.addTodayStat(FoodStat);
                                    cout << "Продукт добавлен!" << endl;
                                }
                                pause();
                            }
                            else if (accCh == '3') {
                                cout << "3" << endl;
                                clearScreen();
                                userHistory.showHistory();
                                pause();
                            }
                            else if (accCh == '4') {
                                cout << "4" << endl;
                                clearScreen();
                                Product customProduct = Product::InputProduct();
                                ProductDB.addProduct(customProduct);
                                cout << "Новый продукт успешно добавлен в глобальную базу!" << endl;
                                pause();
                            }
                            else if (accCh == '5') {
                                cout << "5" << endl;
                                clearScreen();
                                userHistory.addTodayStat(FoodStat);
                                FoodStat.closeDay();
                                cout << "Текущий день сохранен в историю. Дневник на сегодня очищен!" << endl;
                                pause();
                            }
                            else if (accCh == '6') {
                                cout << "6" << endl;
                                clearScreen();
                                ProductDB.sortMenu();
                                pause();
                            }
                            else if (accCh == '7') {
                                cout << "7" << endl;
                                clearScreen();
                                UserStat.editProfile(activeUser);
                                pause();
                            }
                            else {
                                cout << endl << "Неизвестная команда." << endl;
                                pause();
                            }
                        }
                        catch (const EscException&) {
                            cout << endl << "Действие отменено." << endl;
                            pause();
                        }
                    }
                }
                else {
                    pause();
                }
            }
            else if (ch == '2') {
                cout << "2" << endl;
                clearScreen();
                UserStat.registerUser();
                pause();
            }
            else {
                cout << endl << "Неверный пункт меню." << endl;
                pause();
            }
        }
        catch (const EscException&) {
            cout << endl << "Операция отменена пользователем." << endl;
            pause();
        }
    }
}