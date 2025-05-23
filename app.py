import os
import time
import glob
from flask import Flask, redirect, render_template, request, send_file, url_for

# Configure Application
app = Flask(__name__)

# Global variables
filename = ""
ftype = ""

# Folder config (Windows paths)
app.config["FILE_UPLOADS"] = r"C:\Users\jyoti\Desktop\project huffman\Huffman_Coding\uploads"
DOWNLOAD_FOLDER = r"C:\Users\jyoti\Desktop\project huffman\Huffman_Coding\downloads"

@app.route("/")
def home():
    # Delete old files
    for f in glob.glob(os.path.join(app.config["FILE_UPLOADS"], '*')):
        os.remove(f)
    for f in glob.glob(os.path.join(DOWNLOAD_FOLDER, '*')):
        os.remove(f)
    return render_template('home.html')

@app.route("/compress.html", methods=["GET", "POST"])
def compress():
    global filename, ftype

    if request.method == "GET":
        return render_template("compress.html", check=0)

    up_file = request.files["file"]
    if up_file and up_file.filename:
        filename = up_file.filename
        file_path = os.path.join(app.config["FILE_UPLOADS"], filename)
        up_file.save(file_path)

        # Run compression executable
        os.system(f'c.exe "{file_path}"')

        filename_wo_ext = os.path.splitext(filename)[0]
        ftype = "-compressed.bin"
        compressed_path = os.path.join(app.config["FILE_UPLOADS"], filename_wo_ext + ftype)
        dest_path = os.path.join(DOWNLOAD_FOLDER, filename_wo_ext + ftype)

        while not os.path.exists(compressed_path):
            time.sleep(0.1)

        os.replace(compressed_path, dest_path)

        return render_template("compress.html", check=1, success_file=url_for("download_file"))
    else:
        return render_template("compress.html", check=-1)

@app.route("/decompress.html", methods=["GET", "POST"])
def decompress():
    global filename, ftype

    if request.method == "GET":
        return render_template("decompress.html", check=0)

    up_file = request.files["file"]
    if up_file and up_file.filename:
        filename = up_file.filename
        file_path = os.path.join(app.config["FILE_UPLOADS"], filename)
        up_file.save(file_path)

        # Run decompression executable
        os.system(f'd.exe "{file_path}"')

        # Get original extension
        with open(file_path, 'rb') as f:
            ext_len = int.from_bytes(f.read(1), 'big')
            ext = f.read(ext_len).decode('utf-8')
            ftype = f"-decompressed.{ext}"

        filename_wo_dash = filename.split("-")[0]  # More reliable than index()
        decompressed_path = os.path.join(app.config["FILE_UPLOADS"], filename_wo_dash + ftype)
        dest_path = os.path.join(DOWNLOAD_FOLDER, filename_wo_dash + ftype)

        while not os.path.exists(decompressed_path):
            time.sleep(0.1)

        os.replace(decompressed_path, dest_path)

        return render_template("decompress.html", check=1, success_file=url_for("download_file"))
    else:
        return render_template("decompress.html", check=-1)

@app.route("/download")
def download_file():
    global filename, ftype
    if not filename or not ftype:
        return "No file available to download.", 404
    filename_wo = filename.split("-")[0] if "-compressed" in filename else os.path.splitext(filename)[0]
    file_to_send = os.path.join(DOWNLOAD_FOLDER, filename_wo + ftype)
    if not os.path.exists(file_to_send):
        return "File not found", 404
    return send_file(file_to_send, as_attachment=True)

if __name__ == "__main__":
    app.run(debug=True)
