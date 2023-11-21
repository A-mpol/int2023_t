#include "number.h"
#include <iomanip>

int2023_t from_int(int32_t i) {
    int2023_t res;
    uint8_t value = 0;
    if (i < 0) {
        value = 0xff;
    }
    
    res.num[0] = i;
    res.num[1] = i >> 8;
    res.num[2] = i >> 16;
    res.num[3] = i >> 24;
    
    for (int i = 4; i < BYTES; ++i) {
        res.num[i] = value;
    }
    
    return res;
}


int2023_t GetPowNum(int num, int degree) {
    int2023_t res = from_int(num);
    if (degree < 9) {
        res = from_int(num * pow(10, degree));
        return res;
    }
    for (int i = 0; i < degree; ++i) {
        res = res * from_int(10);
    }
    return res;
}


int2023_t from_string(const char* buff) {
    int2023_t res = from_int(0);
    bool is_num_positive = true;
    long len_buff = strlen(buff);
    int num;
    
    if (buff[0] == '-') {
        is_num_positive = false;
        --len_buff;
    }
    
    for (int i = 0; i < len_buff; ++i) {
        num = (int) buff[strlen(buff) - 1 - i] - 48;
        res = res + GetPowNum(num, i);
    }
    
    if (is_num_positive) {
        return res;
    }
    
    return opposite(res);
}


int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t res;
    int16_t full_amount;
    int8_t carry = 0;
    
    for (int i = 0; i < BYTES; ++i) {
        full_amount = lhs.num[i] + rhs.num[i] + carry;
        res.num[i] = full_amount;
        carry = full_amount >> 8;
    }
    
    return res;
}


std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
    bool is_no_null = false;
    for (int i = BYTES - 1; i > -1; --i) {
        if (!is_no_null && value.num[i] != 0) {
            is_no_null = true;
        }
        if (is_no_null) {
            stream << std::setfill('0') << std::setw(2) << std::hex << (int) value.num[i];
        }
    }
    
    if (!is_no_null) {
        stream << 0;
    }
    
    return stream;
}


int2023_t opposite(const int2023_t value) {
    int2023_t res;

    for (int i = 0; i < BYTES; ++i) {
        res.num[i] = ~value.num[i];
    }
    
    res = res + from_int(1);
    
    return res;
}


int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t new_rhs = opposite(rhs);
    
    return lhs + new_rhs;
}


bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
    for (int i = 0; i < BYTES; ++i) {
        if (lhs.num[i] != rhs.num[i]) {
            return false;
        }
    }
    return true;
}


bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
    if (lhs == rhs) {
        return false;
    }
    return true;
}


bool is_positive(const int2023_t value) {
    return (!((value.num[252] & (1 << 6)) > 0));
}


bool operator<(const int2023_t& lhs, const int2023_t& rhs) {
    return (lhs != rhs && is_positive(rhs - lhs));
}


bool operator>(const int2023_t& lhs, const int2023_t& rhs) {
    return (rhs < lhs);
}


int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
    int base = 256;
    int2023_t res = from_int(0);
    int2023_t formatted_lhs = lhs;
    int2023_t formatted_rhs = rhs;
    bool is_num_positive = true;
    
    if (!is_positive(lhs) && !is_positive(rhs)) {
        formatted_lhs = opposite(lhs);
        formatted_rhs = opposite(rhs);
    }
    
    else if (!is_positive(lhs)) {
        formatted_lhs = opposite(lhs);
        is_num_positive = false;
    }
    
    else if (!is_positive(rhs)) {
        formatted_rhs = opposite(rhs);
        is_num_positive = false;
    }
    
    for (int i = 0; i < BYTES; ++i) {
        for (int j = 0, carry = 0; j < BYTES || carry; ++j) {
            long long cur = res.num[i + j] + formatted_lhs.num[i] * 1ll * (j < BYTES ? formatted_rhs.num[j] : 0) + carry;
            res.num[i + j] = int(cur % base);
            carry = int(cur / base);
        }
    }
    
    if (is_num_positive) {
        return res;
    }
    
    return opposite(res);
}


int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
    bool is_num_positive = true;
    int2023_t formatted_lhs = lhs;
    int2023_t formatted_rhs = rhs;
    int2023_t res = from_int(0);
    int2023_t curValue = from_int(0);
    int osn = 0x100;
    int amount = BYTES - 1;
    int l;
    int r;
    int x;
    int m;
    
    if (!is_positive(lhs) && !is_positive(rhs)) {
        formatted_lhs = opposite(lhs);
        formatted_rhs = opposite(rhs);
    }
    
    else if (!is_positive(lhs)) {
        formatted_lhs = opposite(lhs);
        is_num_positive = false;
    }
    
    else if (!is_positive(rhs)) {
        formatted_rhs = opposite(rhs);
        is_num_positive = false;
    }
    
    while (!formatted_lhs.num[amount]) {
           --amount;
    }
    ++amount;
    for (int i = amount - 1; i >= 0; --i) {
        curValue.LevelUp() ;
        curValue.num[0] = formatted_lhs.num[i];
        // подбираем максимальное число x, такое что b * x <= curValue
        x = 0;
        l = 0;
        r = osn;
        while (l <= r) {
            m = (l + r) >> 1;
            int2023_t cur = formatted_rhs * from_int(m);
            if ((cur < curValue) || ( cur == curValue)) {
                x = m;
                l = m + 1;
            }
            else
                r = m - 1;
        }
        res.num[i] = x;
        curValue = curValue - formatted_rhs * from_int(x);
    }
    
    if (!is_num_positive) {
        return opposite(res);
    }
    
    return res;
}
