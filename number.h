#pragma once
#include <cinttypes>
#include <iostream>
static const int BYTES = 253;


struct int2023_t {
    uint8_t num[BYTES];
    void LevelUp() {
        int amount = BYTES - 1;
        for (int i = amount; i >= 1; i--)
            num[i] = num[i - 1];
        if (num[amount])
            amount++;
    }
};

static_assert(sizeof(int2023_t) <= 253, "Size of int2023_t must be no higher than 253 bytes");

int2023_t from_int(int32_t i); // вводим инт получаем инт2023

int2023_t from_string(const char* buff);

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs); 

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs);

bool operator==(const int2023_t& lhs, const int2023_t& rhs);

bool operator!=(const int2023_t& lhs, const int2023_t& rhs);

bool is_positive(const int2023_t& lhs, const int2023_t& rhs); // положительное?

bool operator<(const int2023_t& lhs, const int2023_t& rhs);

bool operator>(const int2023_t& lhs, const int2023_t& rhs);

std::ostream& operator<<(std::ostream& stream, const int2023_t& value);

int2023_t opposite(const int2023_t value); // доп код (инверсия + 1)
