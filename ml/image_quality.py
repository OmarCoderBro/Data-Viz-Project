import cv2


class ImageQualityAnalyzer:
    def blur_score(self, image_path):
        image = cv2.imread(image_path)

        if image is None:
            return 0.0

        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

        return cv2.Laplacian(gray, cv2.CV_64F).var()

    def classify_quality(self, image_path):
        score = self.blur_score(image_path)

        if score < 100:
            return "Low"
        elif score < 500:
            return "Medium"
        else:
            return "High"