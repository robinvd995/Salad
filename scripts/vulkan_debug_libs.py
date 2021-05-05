import os
import subprocess
import sys
import ntpath
import time

import requests
import zipfile
from pathlib import Path

import re

def downloadFile(link, file_name):
    print("Downloading: " + link + " > " + file_name)
    start = time.perf_counter()
    response = requests.get(link, stream=True)
    with(open(file_name, "wb") as f):
        response = requests.get(link, stream=True)
        total_length = int(response.headers.get('content-length'))
        print(total_length / 1024, "Kb")
        print(response.headers["date"])

        if(total_length is None): # no content length header
            f.write(response.content)
        else:
            dl = 0
            for data in response.iter_content(chunk_size=4096):
                dl += len(data)
                f.write(data)
                done = int(50 * dl / total_length)
                sys.stdout.write("\r[%s%s] %s bps" % ('=' * done, ' ' * (50-done), dl//(time.perf_counter() - start)))
                sys.stdout.flush()

REQUIRED_VERSION = "1.2.176.0"
VULKAN_SDK = os.environ.get('VULKAN_SDK')

if(VULKAN_SDK == None):
    print("Can't find VULKAN_SDK enviroment variable, skipping...")
    exit(-1)

print("Do you want to download VulkanSDK " + REQUIRED_VERSION + " [Yes/No]?")
response = input()
if(not (response == "y" or response == "yes")):
    exit(-1)

dir_path = os.path.dirname(os.path.realpath(__file__))
debug_libs_url = "https://sdk.lunarg.com/sdk/download/" + REQUIRED_VERSION + "/windows/VulkanSDK-" + REQUIRED_VERSION + "-DebugLibs.zip"
debug_libs_file = dir_path + "\\vulkan_debug_libs.zip"
downloadFile(debug_libs_url, debug_libs_file)

print("\nExtracting debug libraries >> " + VULKAN_SDK)
with zipfile.ZipFile(debug_libs_file, 'r') as zip_ref:
    zip_ref.extractall(VULKAN_SDK)
print("Finished extracting debug libraries...")

print("Deleting debug libs zip...")
os.remove(debug_libs_file)
