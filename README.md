# Image File Intelligence

Distributed image analysis pipeline built with **React, C++, Redis, Python, PyTorch, OpenCV, and Docker**.

The system uses a queue-based architecture to separate HTTP request handling from ML and image-processing workloads.

---

## Tech Stack

| Layer               | Technology                   |
| ------------------- | ---------------------------- |
| Frontend            | React, Vite, CSS             |
| API / Orchestration | C++, Crow                    |
| Queue               | Redis, redis-plus-plus       |
| ML Worker           | Python, PyTorch, TorchVision |
| Image Processing    | OpenCV, Pillow, ImageHash    |
| Model               | Pretrained ResNet18          |
| Containerization    | Docker, Docker Compose       |
| Build System        | CMake                        |

---

## Architecture

```text id="0ts58t"
┌──────────────┐
│ React / Vite │
│    :5173     │
└──────┬───────┘
       │ POST /analyze
       ▼
┌──────────────┐
│   C++ API    │
│ Crow :8080   │
└──────┬───────┘
       │ LPUSH image_jobs
       ▼
┌──────────────┐
│    Redis     │
│  image_jobs  │
└──────┬───────┘
       │ BLPOP
       ▼
┌──────────────┐
│ Python Worker│
│    Docker    │
└──────┬───────┘
       │ ResNet18 + OpenCV
       ▼
┌──────────────┐
│    Redis     │
│ image_results│
└──────┬───────┘
       │ BLPOP
       ▼
┌──────────────┐
│   C++ API    │
└──────┬───────┘
       │ JSON
       ▼
┌──────────────┐
│    React     │
└──────────────┘
```

---

## System Design

### Request Flow

1. React sends `POST /analyze` to the C++ API.
2. The C++ API pushes the image path to the Redis `image_jobs` queue.
3. The Python worker blocks on `image_jobs`.
4. The worker runs:

   * ResNet18 classification
   * OpenCV image quality analysis
5. The worker serializes the result as JSON.
6. The result is pushed to `image_results`.
7. The C++ API retrieves the result and returns it to React.

### Redis Queues

| Queue           | Producer      | Consumer      | Purpose                    |
| --------------- | ------------- | ------------- | -------------------------- |
| `image_jobs`    | C++ API       | Python Worker | Image analysis jobs        |
| `image_results` | Python Worker | C++ API       | Completed analysis results |

### Worker Communication

```text id="uj6zrt"
C++ API
  │
  ├── LPUSH image_jobs
  ▼
Redis
  │
  ├── BLPOP image_jobs
  ▼
Python Worker
  │
  ├── ML + Image Processing
  │
  └── LPUSH image_results
       ▼
     Redis
       │
       └── BLPOP image_results
            ▼
          C++ API
```

---

## Project Structure

```text id="h47mqs"
image-file-intelligence/
├── backend/
│   ├── main.cpp
│   ├── ImageScanner.cpp
│   ├── ImageMetadata.cpp
│   └── ImageClassifier.cpp
│
├── ml/
│   ├── worker.py
│   ├── image_classifier.py
│   ├── image_quality.py
│   ├── duplicate_detector.py
│   ├── requirements.txt
│   └── Dockerfile
│
├── src/
│   ├── App.jsx
│   └── App.css
│
├── test_images/
├── docker-compose.yml
└── CMakeLists.txt
```

---

## Prerequisites

Install:

* CMake 3.20+
* C++17 compiler
* OpenCV
* Redis++
* Crow
* Docker + Docker Compose
* Node.js + npm

---

## Installation

Clone the repository:

```bash id="pvqptx"
git clone <repository-url>
cd image-file-intelligence
```

Install frontend dependencies:

```bash id="6zo0j8"
npm install
```

---

## Running the Application

### 1. Start Redis and Python Worker

```bash id="he6e04"
docker compose up -d --build
```

Verify:

```bash id="9e8sz2"
docker compose ps
```

Expected services:

```text id="nmh16z"
image-redis
image-worker
```

### 2. Build the C++ API

```bash id="k0alra"
mkdir -p build
cd build
cmake ..
cmake --build .
```

### 3. Start the C++ API

```bash id="9ntuvr"
./image_intelligence
```

API:

```text id="g5t9s8"
http://localhost:8080
```

### 4. Start the React Frontend

From the project root:

```bash id="b9t6s3"
npm run dev
```

Frontend:

```text id="ym07ns"
http://localhost:5173
```

---

## API

### `POST /analyze`

Submits an image analysis job and returns the completed worker result.

**Request**

```text id="pckyd9"
/app/test_images/<image-name>
```

**Response**

```json id="ahjpd8"
{
  "image": "/app/test_images/example.png",
  "predictions": [
    {
      "label": "example_label",
      "confidence": 0.95
    }
  ],
  "quality": "Good"
}
```

---

## Development Commands

### C++ Source Changes

```bash id="j4ptz2"
cd build
cmake --build .
./image_intelligence
```

### CMake Changes

```bash id="1r10bq"
cd build
cmake ..
cmake --build .
./image_intelligence
```

### Python Worker Changes

```bash id="x2erqu"
docker compose up -d --build worker
```

### React Changes

Vite reloads automatically while running:

```bash id="yfzfhv"
npm run dev
```

### Restart Docker Services

```bash id="4dpw51"
docker compose up -d
```

### Stop Docker Services

```bash id="v2jkw1"
docker compose down
```

### View Worker Logs

```bash id="x1bc2u"
docker compose logs -f worker
```

---

## Current Functionality

* React image selection interface
* C++ HTTP API
* Redis-backed job queue
* Dockerized Python worker
* ResNet18 image classification
* Top prediction confidence scores
* OpenCV image quality analysis
* Redis results queue
* End-to-end JSON response to frontend

---

## Current MVP Limitation

The frontend currently sends the selected filename as a path to the API. The corresponding image must exist in:

```text id="2yx3zs"
test_images/
```

which is mounted inside the worker container at:

```text id="5dgn9z"
/app/test_images/
```

Full browser file uploads and persistent object storage are future extensions.
