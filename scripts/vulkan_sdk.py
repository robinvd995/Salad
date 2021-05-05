import os
import subprocess
import sys
import ntpath
import time

import requests
import zipfile
from pathlib import Path

import re

REQUIRED_VERSION = "1.2.176.0"
VULKAN_SDK = os.environ.get('VULKAN_SDK')

if(VULKAN_SDK == None):
    VULKAN_SDK = "0"

def cmp(a, b):
    return (a > b) - (a < b) 

def compareVersions(version1, version2):
    def normalize(v):
        return [int(x) for x in re.sub(r'(\.0+)*$','', v).split(".")]
    return cmp(normalize(version1), normalize(version2))

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

print("VulkanSDK version: " + ntpath.basename(VULKAN_SDK))
versionResult = compareVersions(ntpath.basename(VULKAN_SDK), REQUIRED_VERSION)
if(versionResult == 0):
    print("Vulkan is up-to-date!")
elif(versionResult == -1):
    print("VulkanSDK version to low, " + REQUIRED_VERSION + " version is required!")
    print("Do you want to download VulkanSDK " + REQUIRED_VERSION + " [Yes/No]?")
    response = input()
    if(response == "y" or response == "yes"):
        url = 'https://sdk.lunarg.com/sdk/download/' + REQUIRED_VERSION + '/windows/vulkan_sdk.exe'
        dir_path = os.path.dirname(os.path.realpath(__file__))
        file_path = dir_path + "\\vulkan_sdk.exe"
        downloadFile(url, file_path)
        try:
            print("\nFinished downloading, starting installation...")
            subprocess.check_output(file_path)
            print("Installation finished!")

        except subprocess.CalledProcessError as grepexc:
            print("VulkanSDK installation canceled...")
        
        print("Deleting VulkanSDK installer...")
        os.remove(file_path)
    else:
        print("Skipping VulkanSDK download...")

elif(versionResult == 1):
    print("A newer version of the VulkanSDK is found")
