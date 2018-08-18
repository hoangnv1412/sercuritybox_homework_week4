#pragma once
#include <string>
using namespace std;

class ProvinceModel
{
public:
	ProvinceModel();
	ProvinceModel(int Code) {
		this->code = Code;
	}

	string convertToName(int Code) {
		switch (Code)
		{
		case 1: return "An Giang";
		case 2: return "Ba Ria - Vung Tau";
		case 3: return "Bac Lieu";
		case 4: return "Bac Kan";
		case 5: return "Bac Giang";
		case 6: return "Bac Ninh";
		case 7: return "Ben Tre";
		case 8: return "Binh Duong";
		case 9: return "Binh Dinh";
		case 10: return "Binh Phuoc";
		case 11: return "Binh Thuan";
		case 12: return "Ca Mau";
		case 13: return "Cao Bang";
		case 14: return "Can Tho(TP)";
		case 15: return "Da Nang(TP)";
		case 16: return "Dak Lak";
		case 17: return "Dak Nong";
		case 18: return "Dien Bien";
		case 19: return "Dong Nai";
		case 20: return "Dong Thap";
		case 21: return "Gia Lai";
		case 22: return "Ha Giang";
		case 23: return "Ha Nam";
		case 24: return "Ha Noi(TP)";
		case 25: return "Ha Tay";
		case 26: return "Ha Tinh";
		case 27: return "Hai Duong";
		case 28: return "Hai Phong(TP)";
		case 29: return "Hoa Binh";
		case 30: return "Ho Chi Minh(TP)";
		case 31: return "Hau Giang";
		case 32: return "Hung Yen";
		case 33: return "Khanh Hoa";
		case 34: return "Kien Giang";
		case 35: return "Kon Tum";
		case 36: return "Lai Chau";
		case 37: return "Lao Cai";
		case 38: return "Lang Son";
		case 39: return "Lam Dong";
		case 40: return "Long An";
		case 41: return "Nam Dinh";
		case 42: return "Nghe An";
		case 43: return "Ninh Binh";
		case 44: return "Ninh Thuan";
		case 45: return "Phu Tho";
		case 46: return "Phu Yen";
		case 47: return "Quang Binh";
		case 48: return "Quang Nam";
		case 49: return "Quang Ngai";
		case 50: return "Quang Ninh";
		case 51: return "Quang Tri";
		case 52: return "Soc Trang";
		case 53: return "Son La";
		case 54: return "Tay Ninh";
		case 55: return "Thai Binh";
		case 56: return "Thai Nguyên";
		case 57: return "Thanh Hoa";
		case 58: return "Thua Thien-Hue";
		case 59: return "Tien Giang";
		case 60: return "Tra Vinh";
		case 61: return "Tuyen Quang";
		case 62: return "Vinh Long";
		case 63: return "Vinh Phuc";	
		case 64: return "Yen Bai";
		
		default:
			return "Error : Can't find your province";
			break;
		}

	}

	~ProvinceModel();
private:
	int code;
};

