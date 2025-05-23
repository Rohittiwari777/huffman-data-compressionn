#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
using namespace std;

struct huffnode {
    char character;
    huffnode *left, *right;
};

typedef huffnode* NODE;

NODE newNode(char character) {
    NODE node = new huffnode();
    node->character = character;
    node->left = node->right = nullptr;
    return node;
}

NODE rebuild_tree(const unordered_map<char, string>& code_map) {
    NODE root = newNode('\0');
    for (auto& pair : code_map) {
        char ch = pair.first;
        string code = pair.second;
        NODE current = root;
        for (char bit : code) {
            if (bit == '0') {
                if (!current->left)
                    current->left = newNode('\0');
                current = current->left;
            } else {
                if (!current->right)
                    current->right = newNode('\0');
                current = current->right;
            }
        }
        current->character = ch;
    }
    return root;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <input .bin file>\n";
        return 1;
    }

    ifstream input(argv[1], ios::binary);
    if (!input) {
        cerr << "Cannot open input file!\n";
        return 2;
    }

    char ext_len;
    input.read(&ext_len, 1);
    string ext(ext_len, '\0');
    input.read(&ext[0], ext_len);

    string out_filename = string(argv[1]).substr(0, string(argv[1]).find("-compressed")) + "-decompressed." + ext;
    ofstream output(out_filename, ios::binary);
    if (!output) {
        cerr << "Cannot create output file!\n";
        return 3;
    }

    unordered_map<char, string> code_map;
    while (true) {
        char ch;
        input.read(&ch, 1);
        if (ch == '\0') break;
        string code;
        char c;
        while (input.get(c) && c != '\0')
            code += c;
        code_map[ch] = code;
    }

    char padding_char;
    input.read(&padding_char, 1);
    int padding = padding_char - '0';
    input.ignore(2); // 2 reserved nulls

    string bitstring;
    char byte;
    while (input.read(&byte, 1)) {
        for (int i = 7; i >= 0; --i)
            bitstring += ((byte >> i) & 1) ? '1' : '0';
    }
    bitstring = bitstring.substr(padding);

    NODE root = rebuild_tree(code_map);
    NODE current = root;
    for (char bit : bitstring) {
        current = (bit == '0') ? current->left : current->right;
        if (!current->left && !current->right) {
            output.put(current->character);
            current = root;
        }
    }

    cout << "File decompressed successfully: " << out_filename << endl;
    input.close();
    output.close();
    return 0;
}
