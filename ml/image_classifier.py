import torch
from torchvision import models, transforms
from PIL import Image


class ImageClassifier:
    def __init__(self):
        self.model = models.resnet18(weights="DEFAULT")
        self.model.eval()

        self.transform = transforms.Compose([
            transforms.Resize((224, 224)),
            transforms.ToTensor(),
            transforms.Normalize(
                mean=[0.485, 0.456, 0.406],
                std=[0.229, 0.224, 0.225]
            )
        ])
        print("After transforms", flush=True)

    def predict(self, image_path):
        image = Image.open(image_path).convert("RGB")
        image_tensor = self.transform(image).unsqueeze(0)

        with torch.no_grad():
            output = self.model(image_tensor)

        prediction = torch.argmax(output, dim=1).item()

        return prediction


    def predict_label(self, image_path):
        image = Image.open(image_path).convert("RGB")
        image_tensor = self.transform(image).unsqueeze(0)

        with torch.no_grad():
            output = self.model(image_tensor)

        probabilities = torch.nn.functional.softmax(output[0], dim=0)

        top_probabilities, top_indices = torch.topk(probabilities, 3)

        categories = models.ResNet18_Weights.DEFAULT.meta["categories"]

        results = []

        for probability, index in zip(top_probabilities, top_indices):
            results.append({
                "label": categories[index.item()],
                "confidence": probability.item()
            })

        return results