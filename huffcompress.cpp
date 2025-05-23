#include <iostream>
#include <cstdio>
#include <cmath>
#include <unordered_map>
#include <string>
#include <cstring>
using namespace std;

#define io                        \
    ios_base::sync_with_stdio(0); \
    cin.tie(0);                   \
    cout.tie(0);

unordered_map<char, string> hashmap;

struct huffnode {
    char character;
    unsigned int freq;
    huffnode *left, *right;
};

typedef huffnode* NODE;

struct Minheap {
    unsigned short size, capacity;
    NODE* array;
};

typedef Minheap* HEAP;

NODE newleafnode(char c, int f) {
    NODE tmp = new huffnode();
    tmp->character = c;
    tmp->freq = f;
    tmp->left = tmp->right = nullptr;
    return tmp;
}

NODE newinternalnode(int f) {
    NODE tmp = new huffnode();
    tmp->character = 0;
    tmp->freq = f;
    tmp->left = tmp->right = nullptr;
    return tmp;
}

void swap(NODE* a, int i, int j) {
    NODE tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

void heapify(HEAP heap) {
    int n = heap->size;
    NODE* a = heap->array;

    for (int i = n / 2; i > 0; i--) {
        int j;
        if (2 * i + 1 <= n)
            j = a[2 * i]->freq < a[2 * i + 1]->freq ? 2 * i : 2 * i + 1;
        else
            j = 2 * i;

        if (a[j]->freq < a[i]->freq) {
            swap(a, i, j);
            while (j <= n / 2 && (a[j]->freq > a[2 * j]->freq || (2 * j + 1 <= n && a[j]->freq > a[2 * j + 1]->freq))) {
                int k;
                if (2 * j + 1 <= n)
                    k = a[2 * j]->freq < a[2 * j + 1]->freq ? 2 * j : 2 * j + 1;
                else
                    k = 2 * j;
                swap(a, j, k);
                j = k;
            }
        }
    }
}

NODE mintop(HEAP heap) {
    NODE top = heap->array[1];
    heap->array[1] = heap->array[heap->size--];
    heapify(heap);
    return top;
}

void insertnode(HEAP heap, NODE leftchild, NODE rightchild, int f) {
    NODE tmp = new huffnode();
    tmp->character = '\0';
    tmp->freq = f;
    tmp->left = leftchild;
    tmp->right = rightchild;
    heap->array[++heap->size] = tmp;
    heapify(heap);
}

void huffman_tree(HEAP heap) {
    while (heap->size > 1) {
        NODE left_child = mintop(heap);
        NODE right_child = mintop(heap);
        insertnode(heap, left_child, right_child, left_child->freq + right_child->freq);
    }
}

void assign_codes(NODE root, string str) {
    if (!root->left && !root->right) {
        hashmap[root->character] = str;
    } else {
        assign_codes(root->left, str + "0");
        assign_codes(root->right, str + "1");
    }
}

int main(int argc, char* argv[]) {
    io

    if (argc != 2) {
        cout << "Invalid cmd line arg. Usage: a.exe <input file>\n";
        return 1;
    }

    FILE* input = fopen(argv[1], "rb");
    if (input == nullptr) {
        cout << "Error opening input file\n";
        return 2;
    }

    string in = argv[1];
    string filetype = in.substr(in.find(".") + 1);
    string out = in.substr(0, in.find(".")) + "-compressed.bin";

    FILE* output = fopen(out.c_str(), "wb");
    if (output == nullptr) {
        cout << "Error creating output file\n";
        return 3;
    }

    // Write file extension metadata
    char ext_len = filetype.length();
    fwrite(&ext_len, 1, 1, output);
    fwrite(filetype.c_str(), 1, filetype.length(), output);

    int freq[256] = {0}, file_size = 0;
    unsigned char buff[1];
    while (fread(buff, 1, 1, input)) {
        freq[buff[0]]++;
        file_size++;
    }

    fseek(input, 0, SEEK_SET);

    int unique_chars = 0;
    for (int i = 0; i < 256; ++i)
        if (freq[i]) unique_chars++;

    HEAP heap = new Minheap();
    heap->capacity = unique_chars + 1;
    heap->size = 0;
    heap->array = new NODE[heap->capacity];

    for (int i = 0; i < 256; i++) {
        if (freq[i]) {
            heap->array[++heap->size] = newleafnode(i, freq[i]);
        }
    }

    heapify(heap);
    huffman_tree(heap);
    assign_codes(heap->array[heap->size], "");

    string encoded = "", codes;
    while (fread(buff, 1, 1, input)) {
        encoded += hashmap[buff[0]];
    }

    int padding = 0;
    if (encoded.size() % 8 != 0)
        padding = 8 - (encoded.size() % 8);

    encoded = string(padding, '0') + encoded;

    for (const auto& pair : hashmap) {
        codes = pair.first + pair.second;
        fwrite(codes.c_str(), codes.size() + 1, 1, output);
    }

    char null = '\0';
    char pad_char = padding + '0';
    fwrite(&null, 1, 1, output);
    fwrite(&pad_char, 1, 1, output);
    fwrite(&null, 1, 1, output);
    fwrite(&null, 1, 1, output);

    for (size_t i = 0; i < encoded.size(); i += 8) {
        string byte_str = encoded.substr(i, 8);
        unsigned char val = 0;
        for (int j = 0; j < 8; j++)
            val = val * 2 + (byte_str[j] - '0');
        fwrite(&val, 1, 1, output);
    }

    cout << "Compressed File successfully\n";

    delete[] heap->array;
    delete heap;
    fclose(input);
    fclose(output);
    return 0;
}
