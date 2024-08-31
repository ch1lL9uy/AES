#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#define ui unsigned int
using namespace std;

void checkBits(string&); //kiem tra dau vao va khoa dung 16 ki tu(16 bytes)
void stringToHex(string&, ui); //chuyen tu chuoi ki tu sang mang hex 
/*
	s00 s04 s08 s12
	s01 s05 s09 s13
	s02 s06 s10 s14
	s03 s07 s11 s15

	mang = [s00, s04, s08, s12, s01, s05, s09, s13, s02, s06, s10, s14, s03, s07, s11, s15]
*/
void generateKeyWord(ui, ui, ui, ui,ui); //chuyen khoa thanh cac key word
void keyWordToBytes(ui, ui, ui, ui, ui); //chuyen cac key word thanh key bytes
void rotWord(ui);
void subWord(ui*, int);
void xorRcon(ui, int);
void keyExpansion(ui, ui, ui, ui, ui, int); //Mo rong khoa
void addRoundKey(ui, ui);
void subBytes(ui);
void shiftRows(ui);
ui timeTwo(ui);
ui timeThree(ui);
void mixColumns(ui);
void displayState(ui);// Hien thi duoi dang block (ma tran 4*4)

void checkBits(string& input) {
	while (input.size() != 16) {
		cout << "Chuoi vua nhap khong dung 16 ki tu\n";
		cout << "Nhap lai: ";
		getline(cin, input);
	}
}

void stringToHex(string& plainText, ui input[16]) {

	int i = 0;
	int check = 0;
	stringstream ss;

	for (i; i < plainText.size(); i++) {
		input[((i << 2) + check) % 16] = (int)plainText[i];
		if ((i + 1) % 4 == 0)
			check++;
	}
}

void generateKeyWord(ui w0[4], ui w1[4], ui w2[4], ui w3[4], ui key[16]) {
	int i1 = 0;
	for (int i = 0; i < 16; i+=4) {
		w0[i1] = key[i];
		w1[i1] = key[i + 1];
		w2[i1] = key[i + 2];
		w3[i1] = key[i + 3];
		i1++;
	}
}

void rotWord(ui w3[4]) {
	ui temp = w3[0];
	w3[0] = w3[1];
	w3[1] = w3[2];
	w3[2] = w3[3];
	w3[3] = temp;
}

void subWord(ui *w3, int length) {
	int SBox[] = { 0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
				0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
				0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
				0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
				0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
				0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
				0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
				0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
				0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
				0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
				0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
				0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
				0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
				0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
				0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
				0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
	};

	for (int i = 0; i < length; i++) {
		*(w3 + i) = SBox[*(w3 + i)];
	}

}

void xorRcon(ui w3[4], int round) {
	int Rc[] = {
				0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
				0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39
	};

	w3[0] ^= Rc[round];
}

void keyWordToKeyBytes(ui w0[4], ui w1[4], ui w2[4], ui w3[4], ui key[16]) {
	int i1 = 0;
	for (int i = 0; i < 16; i+=4) {

		key[i] = w0[i1];
		key[i + 1] = w1[i1];
		key[i + 2] = w2[i1];
		key[i + 3] = w3[i1];

		i1++;
	}
}

void keyExpansion(ui w0[4], ui w1[4], ui w2[4], ui w3[4], ui key[16], int round) {

	generateKeyWord(w0, w1, w2, w3, key);

	ui g[4];
	for (int i = 0; i < 4; i++)
		g[i] = w3[i];

	rotWord(g);
	subWord(&g[0], 4);
	xorRcon(g, round);

	for (int i = 0; i < 4; i++) {
		w0[i] ^= g[i];
		w1[i] ^= w0[i];
		w2[i] ^= w1[i];
		w3[i] ^= w2[i];
	}

	keyWordToKeyBytes(w0, w1, w2, w3, key);

}

void addRoundKey(ui state[16], ui key[16]) {
	for (int i = 0; i < 16; i++) {
		state[i] ^= key[i];
	}
}

void subBytes(ui state[16]) {
	subWord(&state[0], 16);
}

void shiftRows(ui state[16]) {
	ui temp[4] = { 0 };
	int check = 24;
	for (int i = 4; i < 16; i++) {
		temp[i / 4] += (state[i] << check);
		check-=8;
		if ((i + 1) % 4 == 0) {
			check = 24;
		}
	}

	temp[1] = ((temp[1] << 8) | (temp[1] >> 24)) & 0xFFFFFFFF;
	temp[2] = ((temp[2] << 16) | (temp[2] >> 16)) & 0xFFFFFFFF;
	temp[3] = ((temp[3] << 24) | (temp[3] >> 8)) & 0xFFFFFFFF;

	check = 0x1000000;
	for (int i = 4; i < 16; i++) {
		state[i] = temp[i / 4] / check;
		temp[i / 4] = temp[i / 4] % check;
		check /= 0x100;
		if ((i + 1) % 4 == 0)
			check = 0x1000000;
	}
}

ui timeTwo(ui w) {
	//Nhan voi 2 hay la dich sang trai 1 bit
	w = w << 1;
	
	//Phep nhan f(x)*g(x) tren GF(2^8) duoc thuc hien bang f(x)*g(x) mod m(x)
	//m(x) = x^8 + x^4 + x3 + x + 1 (0001 0001 1011 = 0x11B)
	if (w & 0x100)
		w ^= 0x11B; //w mod m(x) 

	return w;
}

ui timeThree(ui w) {
	//Nhan voi 3: dich trai 1 bit, sau do thuc hien phep XOR voi gia tri ban dau ((w ^ (w << 1))
	w ^= timeTwo(w);
	
	return w;
}

//Thuc hien phep nhan giua ma tran sinh va state trong GF(2^8)
void mixColumns(ui state[16]) {
	/*
		Ma tran sinh cua GF(2^8)

		ui matrix[16] = {
				0x02, 0x03, 0x01, 0x01, 
				0x01, 0x02, 0x03, 0x01, 
				0x01, 0x01, 0x02, 0x03, 
				0x03, 0x01, 0x01, 0x02
		};
	*/

	ui w[4]; 
	for (int i = 0; i < 4; i++) {

		w[0] = timeTwo(state[i]) ^ timeThree(state[i + 4]) ^ state[i + 8] ^ state[i + 12];
		w[1] = state[i] ^ timeTwo(state[i + 4]) ^ timeThree(state[i + 8]) ^ state[i + 12];
		w[2] = state[i] ^ state[i + 4] ^ timeTwo(state[i + 8]) ^ timeThree(state[i + 12]);
		w[3] = timeThree(state[i]) ^ state[i + 4] ^ state[i + 8] ^ timeTwo(state[i + 12]);

		state[i] = w[0];
		state[i + 4] = w[1];
		state[i + 8] = w[2];
		state[i + 12] = w[3];
	}
}


void displayState(ui state[16]) {
	cout << "\n";
	/*for (int i = 0; i < 16; i++) {
		cout << setw(2) << setfill('0') << hex << state[i] << " ";
		if ((i + 1) % 4 == 0)
			cout << endl;
	}*/

	cout << "Thong tin cua ban da bi ma hoa: ";
	for (int i = 0; i < 4; i++) {
		cout << setw(2) << setfill('0') << hex << state[i];
		cout << setw(2) << setfill('0') << hex << state[i + 4];
		cout << setw(2) << setfill('0') << hex << state[i + 8];
		cout << setw(2) << setfill('0') << hex << state[i + 12];
	}

	cout << "\n";
}

int main() {
	string plainText, k;
	ui state[16], key[16], w0[4], w1[4], w2[4], w3[4];

	cout << "Nhap doan van ban can ma hoa(16 ki tu): ";
	getline(cin, plainText);//nhap van ban can ma hoa
	checkBits(plainText);
	
	cout << "Nhap key(16 ki tu): ";
	getline(cin, k);//nhap key
	checkBits(k);

	//chuyen tu chuoi sang mang so
	stringToHex(plainText, state);
	stringToHex(k, key);


	addRoundKey(state, key);
	/*displayState(state);
	subBytes(state);
	displayState(state);
	shiftRows(state);
	displayState(state);
	mixColumns(state);
	displayState(state);
	addRoundKey(state, key);
	keyExpansion(w0, w1, w2, w3, key, 0);
	displayState(state);*/
	
	//9 round dau tien thuc hien day du
	for (int round = 0; round < 9; round++) {
		subBytes(state);
		shiftRows(state);
		mixColumns(state);
		keyExpansion(w0, w1, w2, w3, key, round);
		addRoundKey(state, key);
	}

	//round 10 khong thuc hien mixColumns
	keyExpansion(w0, w1, w2, w3, key, 9);
	subBytes(state);
	shiftRows(state);
	addRoundKey(state, key);

	displayState(state);
}