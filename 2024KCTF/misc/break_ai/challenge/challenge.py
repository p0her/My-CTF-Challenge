import torch
import io
import base64

try:
    encoded_string = input('base64 encoded model > ')
    io_data = io.BytesIO(base64.b64decode(encoded_string))
    model = torch.load(io_data, map_location=torch.device("cpu"))
    model.eval()
    print('Good model!')

except Exception as e:
    print(f'Model is not good: {e}')
