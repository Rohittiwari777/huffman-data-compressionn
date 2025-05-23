# Data Compression Tool

This is a web-based data compression application that allows users to upload files for compression and decompression using Huffman Coding. It supports file uploads, processes them on the backend, and returns the compressed or decompressed file for download.

## Features

- 🔄 Compress and decompress files using Huffman Encoding
- 🌐 Web interface built with HTML, CSS, and Flask
- 📤 Upload and download files directly from the browser
- 🧠 Core compression logic written in C++ for efficiency
- ❌ No persistent file storage — all processing is done in-memory

## Technologies Used

- **Frontend:** HTML, CSS
- **Backend:** Python (Flask)
- **Compression Engine:** C++
- **Other:** JavaScript (for UX), Python subprocess to run C++ code

## Project Structure

data-compression/
├── app.py # Flask backend
├── templates/
│ └── index.html # Frontend HTML page
├── static/
│ └── style.css # CSS styling
├── compression/
│ ├── compress.cpp # Huffman compression logic
│ ├── decompress.cpp # Huffman decompression logic
│ └── Makefile # Compiles C++ files
└── uploads/ # Temporary folder for file uploads



## How to Run Locally

### 1. Clone the repository

```bash
git clone https://github.com/yourusername/data-compression.git
cd data-compression



##2. Install Python dependencies
pip install -r requirements.txt


##3. Compile the C++ files
cd compression
make
cd ..


##4. Start the Flask server
python app.py


##5. Open in your browser
Navigate to http://localhost:5000 to use the tool.

##Future Enhancements

Add user login and history tracking
Store compressed files with metadata
Improve compression algorithm options (e.g., LZ77, Deflate)


