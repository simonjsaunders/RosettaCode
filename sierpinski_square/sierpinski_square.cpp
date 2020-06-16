// See https://en.wikipedia.org/wiki/Sierpi%C5%84ski_curve#Representation_as_Lindenmayer_system
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

std::string rewrite(const std::string& s) {
    std::string t;
    for (char c : s) {
        if (c == 'X')
            t += "XF-F+F-XF+F+XF-F+F-X";
        else
            t += c;
    }
    return t;
}

void line(std::ostream& out, double& x, double& y, double length, int angle) {
    constexpr double pi = 3.14159265359;
    double theta = (pi * angle)/180.0;
    x += length * std::cos(theta);
    y += length * std::sin(theta);
    out << 'L' << x << ',' << y << '\n';
}

void execute(std::ostream& out, const std::string& s, double x, double y,
             double length, int angle) {
    out << 'M' << x << ',' << y << '\n';
    for (char c : s) {
        if (c == 'F')
            line(out, x, y, length, angle);
        else if (c == '+')
            angle = (angle + 90) % 360;
        else if (c == '-')
            angle = (angle - 90) % 360;
    }
}

int main() {
    const int size = 635, length = 5;
    const int order = 5;
    std::ofstream out("sierpinski_square.svg");
    if (!out) {
        std::cerr << "Cannot open output file\n";
        return 1;
    }
    out << "<svg xmlns='http://www.w3.org/2000/svg' width='"
        << size << "' height='" << size << "'>\n";
    out << "<rect width='100%' height='100%' fill='white'/>\n";
    out << "<path stroke-width='1' stroke='black' fill='none' d='";
    std::string s = "F+XF+F+XF";
    for (int i = 0; i < order; ++i)
        s = rewrite(s);
    execute(out, s, (size - length)/2, length, length, 0);
    out << "'/>\n</svg>\n";
    return 0;
}
