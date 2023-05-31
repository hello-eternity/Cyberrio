
typedef long long ditype;
typedef unsigned long long uditype;

uditype __umuldi3(uditype a, uditype b) {
    if (b == 0 || a == 0) {
        return 0;
    } else {
        uditype ret = 0;
        while (b > 1) {
            if (b % 2 == 1) {
                ret += a;
            }
            a *= 2;
            b /= 2;
        }
        return ret + a;
    }
}

ditype __muldi3(ditype a, ditype b) {
    return __umuldi3(a, b);
}

typedef long sitype;
typedef unsigned long usitype;

usitype __umulsi3(usitype a, usitype b) {
    if (b == 0 || a == 0) {
        return 0;
    } else {
        usitype ret = 0;
        while (b > 1) {
            if (b % 2 == 1) {
                ret += a;
            }
            a *= 2;
            b /= 2;
        }
        return ret + a;
    }
}

sitype __mulsi3(sitype a, sitype b) {
    return __umulsi3(a, b);
}

typedef struct {
    sitype q;
    sitype r;
} sidiv_result;

typedef struct {
    usitype q;
    usitype r;
} usidiv_result;

usidiv_result __usidivide(usitype n, usitype d) {
    if (d == 0) {
        usidiv_result tmp = { 0xffffffff, n };
        return tmp;
    } else {
        usidiv_result tmp = { 0, 0 };
        for (int i = 31; i >= 0; i--) {
            tmp.r *= 2;
            tmp.r |= (n >> i) & 1;
            if (tmp.r >= d) {
                tmp.r -= d;
                tmp.q |= 1 << i;
            }
        }
        return tmp;
    }
}

sidiv_result __sidivide(sitype n, sitype d) {
    sidiv_result ret = { -1, n };
    if (d != 0) {
        if (d < 0) {
            sidiv_result tmp = __sidivide(n, -d);
            ret.q = -tmp.q;
            ret.r = tmp.r;
        } else if (n < 0) {
            sidiv_result tmp = __sidivide(-n, d);
            if (tmp.r == 0) {
                ret.q = -tmp.q;
                ret.r = tmp.r;
            } else {
                ret.q = -tmp.q - 1;
                ret.r = d - tmp.r;
            }
        } else {
            usidiv_result tmp = __usidivide(n, d);
            ret.q = tmp.q;
            ret.r = tmp.r;
        }
    }
    return ret;
}

sitype __modsi3(sitype a, sitype b) {
    sidiv_result res = __sidivide(a, b);
    return res.r;
}

