# Image & PDF Compression Web Application

## 1. Project Description
A modern web application that allows users to compress images (JPEG/PNG) and PDF files directly in the browser. The app features a beautiful, animated UI, a loading bar, and celebratory confetti when compression is complete.

## 2. Tech Stack & Why It Was Chosen

### Frontend
- **React (with Vite):** Fast, component-based, easy to build interactive UIs, and Vite offers rapid development and build times.
- **CSS (custom, with Google Fonts):** For custom, beautiful, and responsive styling.
- **Vanta.js + three.js:** For animated, interactive backgrounds (modern, eye-catching).
- **Axios:** For easy HTTP requests to the backend.
- **Party popper/confetti animation (custom CSS/JS):** To enhance user experience and provide feedback on successful compression.

### Backend
- **FastAPI (Python):** Modern, fast, async-ready, easy to use for building APIs, and great for serving both images and PDFs.
- **Pillow (Python Imaging Library):** Powerful, well-supported library for image processing and compression.
- **pikepdf:** Efficient, Pythonic library for manipulating and compressing PDF files.
- **Uvicorn:** High-performance ASGI server for running FastAPI in production or development.

## 3. How Compression Works

### Image Compression
- **Library Used:** Pillow
- **Technique:**  
  - For JPEG: Adjusts the `quality` parameter (lower quality = higher compression, smaller file).
  - For PNG: Adjusts the `compress_level` (0-9) and uses optimization.
- **Advantage:**  
  - Lossy (JPEG) or lossless (PNG) compression, with user control over quality/size tradeoff.
  - Fast, works for most common image formats.

### PDF Compression
- **Library Used:** pikepdf
- **Technique:**  
  - Rewrites the PDF, removing unused objects and streams, and re-compresses streams using default settings.
- **Advantage:**  
  - Reduces file size for many PDFs, especially those with redundant or unoptimized content.
  - Works for most standard PDFs.

## 4. UI/UX Features
- Modern glassmorphism card design
- Animated Vanta.js background
- Animated, interactive buttons
- Loading bar during compression
- Party popper/confetti animation on success
- Responsive and accessible design
- File type selector (Image/PDF)
- Image quality slider for images
- PDF preview for compressed PDFs

## 5. How the App Works (Workflow)
1. User selects file type (Image or PDF).
2. User uploads a file.
3. (For images) User selects desired quality.
4. User clicks "Compress".
5. App shows a loading bar while compressing.
6. On success, a download link and preview are shown, with a confetti animation.

## 6. Advantages of This Approach
- User-friendly: Simple, beautiful, and interactive.
- Fast: Uses efficient libraries and async backend.
- Flexible: Supports both images and PDFs.
- Modern: Uses up-to-date frameworks and libraries.
- Portable: Can be deployed on any cloud or local server.

## 7. Possible Teacher Questions & Answers

**Q: Why use FastAPI instead of Flask or Django?**  
A: FastAPI is async-ready, faster, and has automatic docs generation. It's modern and easy to use for APIs.

**Q: Why Pillow for images?**  
A: Pillow is the de facto standard for image processing in Python, supporting many formats and compression options.

**Q: Why pikepdf for PDFs?**  
A: pikepdf is efficient, Pythonic, and can compress, optimize, and manipulate PDFs easily.

**Q: How does image compression work?**  
A: For JPEG, it reduces quality (lossy). For PNG, it increases compression level (lossless).

**Q: How does PDF compression work?**  
A: It rewrites the PDF, removing unused objects and re-compressing streams.

**Q: What is Vanta.js?**  
A: A JavaScript library for animated, interactive backgrounds, used here for a modern look.

**Q: How is user experience enhanced?**  
A: With animations, loading bar, confetti, and a clean, responsive UI.

**Q: Can this be extended?**  
A: Yes! You can add more file types, advanced compression, authentication, or cloud storage.

## 8. Project Structure
```
image-compression/
  backend/
    main.py
    (venv, requirements.txt, etc.)
  frontend/
    src/
      App.jsx
      App.css
    dist/ (build output)
    package.json
```

## 9. How to Run Locally
1. **Backend:**  
   - `cd backend`  
   - `python3 -m venv venv && source venv/bin/activate`  
   - `pip install fastapi uvicorn pillow pikepdf`  
   - `uvicorn main:app --reload`
2. **Frontend:**  
   - `cd frontend`  
   - `npm install`  
   - `npm run dev`
3. Open the frontend in your browser (usually http://localhost:5173).

## 10. How to Deploy
- See previous message for deployment options (Vercel, Render, etc.).

## 11. References
- [FastAPI](https://fastapi.tiangolo.com/)
- [Pillow](https://python-pillow.org/)
- [pikepdf](https://pikepdf.readthedocs.io/)
- [Vanta.js](https://www.vantajs.com/)
- [React](https://react.dev/)
- [Vite](https://vitejs.dev/) 