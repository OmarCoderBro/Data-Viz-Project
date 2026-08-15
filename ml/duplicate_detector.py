from PIL import Image
import imagehash


class DuplicateDetector:
    def hash_image(self, image_path):
        image = Image.open(image_path)
        return imagehash.phash(image)

    def compare(self, image_path_1, image_path_2):
        hash_1 = self.hash_image(image_path_1)
        hash_2 = self.hash_image(image_path_2)

        return hash_1 - hash_2

    def is_duplicate(self, image_path_1, image_path_2, threshold=5):
        distance = self.compare(image_path_1, image_path_2)
        return distance <= threshold