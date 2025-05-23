import { useState, useRef, useEffect } from 'react'
import axios from 'axios'
import './App.css'
import * as THREE from 'three'
import NET from 'vanta/dist/vanta.net.min'

function App() {
  const [fileType, setFileType] = useState('image')
  const [file, setFile] = useState(null)
  const [quality, setQuality] = useState(75)
  const [resultUrl, setResultUrl] = useState(null)
  const [loading, setLoading] = useState(false)
  const [error, setError] = useState('')
  const [showPoppers, setShowPoppers] = useState(false)
  const vantaRef = useRef(null)
  const vantaEffect = useRef(null)

  useEffect(() => {
    if (!vantaEffect.current) {
      vantaEffect.current = NET({
        el: vantaRef.current,
        THREE: THREE,
        mouseControls: true,
        touchControls: true,
        minHeight: 200.00,
        minWidth: 200.00,
        scale: 1.0,
        scaleMobile: 1.0,
        color: 0x6366f1,
        backgroundColor: 0xe0e7ff,
        points: 12.0,
        maxDistance: 22.0,
        spacing: 18.0,
      })
    }
    return () => {
      if (vantaEffect.current) {
        vantaEffect.current.destroy()
        vantaEffect.current = null
      }
    }
  }, [])

  const handleFileTypeChange = (e) => {
    setFileType(e.target.value)
    setFile(null)
    setResultUrl(null)
    setError('')
  }

  const handleFileChange = (e) => {
    setFile(e.target.files[0])
    setResultUrl(null)
    setError('')
  }

  const handleCompress = async () => {
    if (!file) {
      setError('Please select a file.')
      return
    }
    setLoading(true)
    setError('')
    setResultUrl(null)
    setShowPoppers(false)
    try {
      const formData = new FormData()
      formData.append('file', file)
      let response, url
      if (fileType === 'image') {
        formData.append('quality', quality)
        response = await axios.post('http://127.0.0.1:8000/compress', formData, {
          responseType: 'blob',
        })
        url = window.URL.createObjectURL(new Blob([response.data]))
      } else {
        response = await axios.post('http://127.0.0.1:8000/compress-pdf', formData, {
          responseType: 'blob',
        })
        url = window.URL.createObjectURL(new Blob([response.data], { type: 'application/pdf' }))
      }
      setResultUrl(url)
      setShowPoppers(true)
      setTimeout(() => setShowPoppers(false), 1200)
    } catch (err) {
      setError('Compression failed.')
    } finally {
      setLoading(false)
    }
  }

  // Generate multiple poppers for effect
  const renderPoppers = () => {
    if (!showPoppers) return null
    const poppers = []
    for (let i = 0; i < 12; i++) {
      const left = 10 + Math.random() * 80
      const delay = Math.random() * 0.3
      poppers.push(
        <span
          className="popper"
          key={i}
          style={{ left: `${left}%`, animationDelay: `${delay}s` }}
        >
          🎉
        </span>
      )
    }
    return poppers
  }

  return (
    <div ref={vantaRef} style={{ minHeight: '100vh', width: '100vw', position: 'fixed', top: 0, left: 0, zIndex: 0 }}>
      {loading && <div className="loading-bar"></div>}
      <div style={{ position: 'relative', zIndex: 1, minHeight: '100vh', display: 'flex', alignItems: 'center', justifyContent: 'center' }}>
        <div className="container" style={{ position: 'relative' }}>
          {renderPoppers()}
          <h1>File Compression App</h1>
          <div className="subtitle">Compress your images or PDFs to save space. Simple, fast, and beautiful.</div>
          <div style={{ marginBottom: 18, width: '100%' }}>
            <select value={fileType} onChange={handleFileTypeChange} style={{ padding: '0.5em', borderRadius: 8, border: '1px solid #c7d2fe', fontSize: '1em', width: '100%' }}>
              <option value="image">Image</option>
              <option value="pdf">PDF</option>
            </select>
          </div>
          <input
            type="file"
            accept={fileType === 'image' ? 'image/*' : 'application/pdf'}
            onChange={handleFileChange}
          />
          {fileType === 'image' && (
            <div style={{ margin: '20px 0', width: '100%', display: 'flex', alignItems: 'center', justifyContent: 'center' }}>
              <label>Quality: {quality}</label>
              <input
                type="range"
                min="10"
                max="100"
                value={quality}
                onChange={e => setQuality(Number(e.target.value))}
                disabled={loading}
              />
            </div>
          )}
          <div className="button-group">
            <button onClick={handleCompress} disabled={loading}>
              {loading ? 'Compressing...' : `Compress ${fileType === 'image' ? 'Image' : 'PDF'}`}
            </button>
          </div>
          {error && <p className="error-message">{error}</p>}
          {resultUrl && (
            <div className="result-section">
              <a href={resultUrl} download={fileType === 'image' ? 'compressed-image' : 'compressed.pdf'}>
                Download Compressed {fileType === 'image' ? 'Image' : 'PDF'}
              </a>
              <div>
                {fileType === 'image' ? (
                  <img src={resultUrl} alt="Compressed Preview" />
                ) : (
                  <embed src={resultUrl} type="application/pdf" width="100%" height="220px" style={{ borderRadius: 12, background: '#f8fafc99', boxShadow: '0 2px 12px #6366f122', border: '1.5px solid #e0e7ff' }} />
                )}
              </div>
            </div>
          )}
        </div>
      </div>
    </div>
  )
}

export default App
