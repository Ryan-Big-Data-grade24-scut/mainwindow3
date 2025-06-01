from paddleocr import PaddleOCR

def init_model():
    return PaddleOCR(use_doc_orientation_classify=False,
    use_doc_unwarping=False,
    use_textline_orientation=False)

def predict_image(model, image_path):
    output = model.ocr(image_path)
    results = []
    res = output[0]["rec_texts"]
    """ 
    results.append({
        'text': res.text,
        'score': res.score,
        'position': res.position
    })
    """
    """
    res.save_to_img(save_path="./output/")
    res.save_to_json(save_path="./output/res.json")
    """
    return res

if __name__ == "__main__":
    model = init_model()
    predict_image(model, "test1.jpg")