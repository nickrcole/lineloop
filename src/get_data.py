import requests
import time
import json

def fetch_and_save_data():
    url = "https://gobool.app/bool/lineloop"
    response = requests.get(url)
    if response.status_code == 200:
        data = response.json()
        with open("preds.json", "w") as file:
            json.dump(data, file, indent=4)
    else:
        print(f"Failed to fetch data. Status code: {response.status_code}")

def connection_failed():
    with open("preds.json", "w") as file:
        json.dump({"predString": "Can't Connect to Bool"}, file, indent=4)

def main():
    while True:
        try:
            fetch_and_save_data()
        except:
            connection_failed()
        time.sleep(1)

if __name__ == "__main__":
    main()
