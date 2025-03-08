#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdint.h>
#include <sys/stat.h>


#define OPCODE_MASK 0b11111100
#define MOV_OPCODE 0b00100010
#define D_MASK 0b00000010
#define W_MASK 0b00000001
#define MOD_MASK 0b11000000
#define REG_MASK 0b00111000
#define RM_MASK 0b00000111

const int BUFFERSIZE = 4096; 

std::vector<std::vector<std::string>> reg_table = {
    {"al", "ax"}, 
    {"cl", "cx"},
    {"dl", "dx"},
    {"bl", "bx"},
    {"ah", "sp"},
    {"ch", "bp"},
    {"dh", "si"},
    {"bh", "di"},
};

struct Instruction {
    uint8_t op_code;
    uint8_t d;
    uint8_t w;
    uint8_t mod;
    uint8_t reg;
    uint8_t rm;
};


void print_byte(uint8_t byte) {
    std::string binaryStr = "0b";
    for (int j = 7; j >= 0; --j) {
        binaryStr += ((byte >> j) & 1) ? '1' : '0';
    }
    std::cout << "processed: " << binaryStr << '\n';
        std::cout << '\n';
}

void process_instruction(uint8_t byte_1, uint8_t byte_2) {
    Instruction inst = {
        .op_code = (static_cast<uint8_t>((byte_1 & OPCODE_MASK) >> 2)),
        .d = (static_cast<uint8_t>((byte_1 & D_MASK) >> 1)),
        .w = (static_cast<uint8_t>((byte_1 & W_MASK) >> 0)),
        .mod = (static_cast<uint8_t>((byte_2 & MOD_MASK) >> 6)),
        .reg = (static_cast<uint8_t>((byte_2 & REG_MASK) >> 3)),
        .rm = (static_cast<uint8_t>((byte_2 & RM_MASK) >> 0)),
    };

    switch(inst.op_code) {
        case MOV_OPCODE:
            std::string src_reg = reg_table.at(inst.reg).at(inst.w);
            std::string dest_reg = reg_table.at(inst.rm).at(inst.w); 

            if (inst.d == 1) {
               std::swap(src_reg, dest_reg);
            }
            printf("%s, %s, %s\n", "mov", dest_reg.c_str(), src_reg.c_str());

            break;
    }    
}

void decode_8086(std::string file_name) {

    char buffer[BUFFERSIZE];
    std::ifstream file(file_name, std::ios::in | std::ios::binary);
    file.read(buffer, BUFFERSIZE);

    std::streamsize bytes_read = file.gcount();
    // std::cout << bytes_read << '\n';

    for (auto i = 0; i < bytes_read; i+= 2 ) {

      process_instruction(static_cast<uint8_t>(buffer[i]), static_cast<uint8_t>(buffer[i+1]));
    }
}

int main() {
    decode_8086("listing_0038_many_register_mov");
    return 0;
}

// void decode_asm_to_binary(std::string file_name) {
//     std::fstream file;
//     file.open(file_name, std::ios::in);
//     if (!file) {
//         std::cout<< "File is not present" << "\n";
//     } else {
//         std::string x;
//         while (std::getline(file, x)) {
//             if (x.size() > 0 && x.at(0) == ';') {
//                 continue;
//             }
//             std::cout<< x << '\n';
//         }
//     }

//     file.close();
// }
