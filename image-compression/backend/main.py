from fastapi import FastAPI, File, UploadFile, Form
from fastapi.responses import StreamingResponse
from PIL import Image
import io
from fastapi.middleware.cors import CORSMiddleware
import pikepdf

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.post("/compress")
async def compress_image(
    file: UploadFile = File(...),
    quality: int = Form(75)
):
    # Read image into memory
    image_bytes = await file.read()
    image = Image.open(io.BytesIO(image_bytes))

    # Prepare output buffer
    output_io = io.BytesIO()
    format = image.format if image.format in ["JPEG", "PNG"] else "JPEG"
    save_kwargs = {"format": format}
    if format == "JPEG":
        save_kwargs["quality"] = quality
        save_kwargs["optimize"] = True
    elif format == "PNG":
        save_kwargs["optimize"] = True
        save_kwargs["compress_level"] = int((100 - quality) / 10)  # Pillow: 0 (best) - 9 (worst)
    image.save(output_io, **save_kwargs)
    output_io.seek(0)

    return StreamingResponse(output_io, media_type=f"image/{format.lower()}", headers={
        "Content-Disposition": f"attachment; filename=compressed.{format.lower()}"
    })

@app.post("/compress-pdf")
async def compress_pdf(file: UploadFile = File(...)):
    # Read PDF into memory
    input_pdf_bytes = await file.read()
    input_pdf_io = io.BytesIO(input_pdf_bytes)
    output_pdf_io = io.BytesIO()
    # Compress PDF using pikepdf (default save)
    with pikepdf.open(input_pdf_io) as pdf:
        pdf.save(output_pdf_io)
    output_pdf_io.seek(0)
    return StreamingResponse(output_pdf_io, media_type="application/pdf", headers={
        "Content-Disposition": "attachment; filename=compressed.pdf"
    })

@app.post("/decompress")
async def decompress_image(file: UploadFile = File(...)):
    # For standard formats, decompression is just returning the file
    image_bytes = await file.read()
    image = Image.open(io.BytesIO(image_bytes))
    output_io = io.BytesIO()
    format = image.format if image.format in ["JPEG", "PNG"] else "JPEG"
    image.save(output_io, format=format)
    output_io.seek(0)
    return StreamingResponse(output_io, media_type=f"image/{format.lower()}", headers={
        "Content-Disposition": f"attachment; filename=decompressed.{format.lower()}"
    })
