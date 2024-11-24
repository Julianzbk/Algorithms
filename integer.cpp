#include <iostream>
#include <cmath>
using std::cout, std::endl;

int multiply_bad(int A, int B)
{
    int N = std::log10(A);
    if ((int) std::log10(B) != N)
        throw std::domain_error("Numbers must have the same length");
    int split_place = pow(10, N/2);
    int A1 = A / split_place;
    int A2 = A % split_place;
    int B1 = B / split_place;
    int B2 = B % split_place;
    int P1 = (A1 * B1) * pow(10, N); // 4 *'s
    int P2 = (A1 * B2 + A2 * B1) * split_place;
    int P3 = A2 * B2;
    return P1 + P2 + P3;
}

int multiply_good(int A, int B)
{
    int N = std::log10(A);
    if ((int) std::log10(B) != N)
        throw std::domain_error("Numbers must have the same length");
    int split_place = pow(10, N/2);
    int A1 = A / split_place;
    int A2 = A % split_place;
    int B1 = B / split_place;
    int B2 = B % split_place;
    int U = (A1 + A2) * (B1 + B2); // 3 *'s
    int V = A1 * B1;
    int W = A2 * B2;
    return V * pow(10, N) - (U - V - W) * split_place + W; // # * pow(10, N) is cheap for humans
}

int quiz(int A, int B)
{
    cout << A << " x " << B << " = ?" << endl;
    int user_answer;
    while (true)
    {
        std::cin >> user_answer;
        if (user_answer == A * B)
            break;
        else
            cout << "Incorrect!\n";
    }
    cout << "Correct!" << endl;
    return user_answer;
}

int multiply_game(int A, int B)
{
    cout << A << " " << B << endl;
    int N = std::log10(A);
    if ((int) std::log10(B) != N)
        throw std::domain_error("Numbers must have the same length");
    if (N <= 2)
        return A * B;
    int split_place = pow(10, N/2);
    int A1 = A / split_place;
    int A2 = A % split_place;
    int B1 = B / split_place;   
    int B2 = B % split_place;
    int U = multiply_game(A1 + A2, B1 + B2);
    int V = multiply_game(A1, B1);
    int W = multiply_game(A2, B2);
    return V * pow(10, N) - (U - V - W) * split_place + W; 
}

int main(int argc, char* argv[])
{
    cout << multiply_game(1234, 5678) << endl;
}
