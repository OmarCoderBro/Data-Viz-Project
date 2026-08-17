from image_classifier import ImageClassifier
from image_quality import ImageQualityAnalyzer
from duplicate_detector import DuplicateDetector
import json
import redis


class ImageWorker:
    def __init__(self):
        print("Starting ImageClassifier...", flush=True)
        self.classifier = ImageClassifier()
        print("Classifier initialized", flush=True)
        self.quality_analyzer = ImageQualityAnalyzer()
        print("Quality analyzer initialized", flush=True)
        self.duplicate_detector = DuplicateDetector()
        print("Duplicate detector initialized", flush=True)
        self.redis_client = redis.Redis(host="image-redis", port=6379, decode_responses=True, socket_timeout=None)
        print("Redis client initialized", flush=True)

    def process_image(self, image_path):
        predictions = self.classifier.predict_label(image_path)
        quality = self.quality_analyzer.classify_quality(image_path)

        return {
            "image": image_path,
            "predictions": predictions,
            "quality": quality
        }

    def process_image_json(self, image_path):
        print("Processing image:", image_path, flush=True)

        predictions = self.classifier.predict_label(image_path)
        print("Classification complete", flush=True)

        quality = self.quality_analyzer.classify_quality(image_path)
        print("Quality analysis complete", flush=True)
        result = self.process_image(image_path)
        return json.dumps(result, indent=2)

    def run(self):
        print("Worker waiting for jobs...", flush=True)

        while True:
            job = self.redis_client.blpop("image_jobs", timeout=0)

            if job:
                image_path = job[1]

                result = self.process_image_json(image_path)

                self.redis_client.lpush("image_results", result)

                print("Result submitted to Redis.", flush=True)


if __name__ == "__main__":
    worker = ImageWorker()
    worker.run()