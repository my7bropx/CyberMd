// Test file for CyberMD syntax highlighting
// This file contains various C++ constructs to test syntax colors

#include <iostream>
#include <string>
#include <vector>
#include <memory>

// This is a single-line comment - should be GREEN

/*
 * This is a multi-line comment
 * Should also be GREEN and italic
 */

// Namespace and class definitions
namespace test {

// Class with various members - "TestClass" should be CYAN
class TestClass {
private:
    int number_;              // Keywords (int, private) should be BLUE
    std::string text_;        // Class names should be CYAN
    double value_;
    
public:
    // Constructor
    TestClass(int num, const std::string& txt) 
        : number_(num), text_(txt), value_(0.0) {
        std::cout << "Constructor called" << std::endl;  // Strings should be ORANGE
    }
    
    // Member function - function names should be YELLOW
    void printInfo() {
        std::cout << "Number: " << number_ << std::endl;
        std::cout << "Text: " << text_ << std::endl;
        std::cout << "Value: " << value_ << std::endl;
    }
    
    // Getter - return keyword should be BLUE
    int getNumber() const {
        return number_;
    }
    
    // Setter with keywords
    void setNumber(int num) {
        if (num > 0) {           // if should be BLUE
            number_ = num;
        } else {                  // else should be BLUE
            number_ = 0;
        }
    }
    
    // Template function
    template<typename T>
    T calculate(T a, T b) {
        return a + b;             // Operators should be visible
    }
    
    // Virtual destructor
    virtual ~TestClass() {
        std::cout << "Destructor called" << std::endl;
    }
};

// Derived class
class DerivedClass : public TestClass {
public:
    DerivedClass() : TestClass(42, "derived") {}
    
    void specialFunction() override {
        std::cout << "Override function" << std::endl;
    }
};

} // namespace test

// Enum definition
enum class Color {
    Red,
    Green,
    Blue,
    Yellow
};

// Function with various keywords
int main(int argc, char* argv[]) {
    // Numbers should be LIGHT GREEN
    int x = 42;
    double pi = 3.14159;
    float f = 2.5f;
    
    // String literals should be ORANGE
    std::string message = "Hello, CyberMD!";
    const char* text = "Testing syntax highlighting";
    
    // Boolean literals
    bool isActive = true;
    bool isDisabled = false;
    
    // Control flow keywords should be BLUE
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            std::cout << i << " is even" << std::endl;
        } else {
            continue;
        }
    }
    
    // While loop
    int counter = 0;
    while (counter < 5) {
        counter++;
    }
    
    // Switch statement
    switch (x) {
        case 42:
            std::cout << "The answer!" << std::endl;
            break;
        case 0:
            std::cout << "Zero" << std::endl;
            break;
        default:
            std::cout << "Something else" << std::endl;
            break;
    }
    
    // Pointer and reference
    int* ptr = &x;
    int& ref = x;
    
    // Smart pointers
    auto smartPtr = std::make_unique<test::TestClass>(100, "smart");
    std::shared_ptr<test::TestClass> sharedPtr = std::make_shared<test::TestClass>(200, "shared");
    
    // Lambda function
    auto lambda = [](int a, int b) -> int {
        return a + b;
    };
    
    int result = lambda(5, 10);
    
    // Try-catch block
    try {
        throw std::runtime_error("Test exception");
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    
    // Modern C++ features
    constexpr int SIZE = 100;
    static_assert(SIZE > 0, "Size must be positive");
    
    // Range-based for loop
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    // Nullptr
    int* nullPtr = nullptr;
    
    // Casting
    double d = 3.14;
    int i = static_cast<int>(d);
    
    return 0;  // return should be BLUE, 0 should be LIGHT GREEN
}

// Expected colors:
// - Keywords (if, for, while, class, return, etc.): BLUE
// - Class names (TestClass, DerivedClass, Color): CYAN  
// - Function names (main, printInfo, calculate): YELLOW
// - Strings ("Hello", "Testing", etc.): ORANGE
// - Numbers (42, 3.14, 100): LIGHT GREEN
// - Comments (// and /* */): GREEN
// - Operators (+, -, *, ==, etc.): DEFAULT/LIGHT GRAY
// - Preprocessor (#include, #define): PURPLE