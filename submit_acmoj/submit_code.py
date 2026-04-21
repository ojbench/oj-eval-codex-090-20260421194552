#!/usr/bin/env python3
# Submit raw code content to ACMOJ
import argparse
import os
import requests
import json

parser = argparse.ArgumentParser()
parser.add_argument('--token', default=os.environ.get('ACMOJ_TOKEN'))
parser.add_argument('--problem-id', type=int, required=True)
parser.add_argument('--language', type=str, default='git')
parser.add_argument('--code-file', type=str, required=True)
args = parser.parse_args()

if not args.token:
    print('Error: token missing')
    raise SystemExit(1)

with open(args.code_file, 'r') as f:
    code = f.read()

url = fhttps://acm.sjtu.edu.cn/OnlineJudge/api/v1/problem/{args.problem_id}/submit
headers = {
    'Authorization': f'Bearer {args.token}',
    'Content-Type': 'application/x-www-form-urlencoded',
    'User-Agent': 'ACMOJ-Python-Client/2.2'
}

data = {
    'language': args.language,
    'code': code
}

try:
    r = requests.post(url, headers=headers, data=data, timeout=15, proxies={'https': None, 'http': None})
    r.raise_for_status()
    j = r.json()
    print(json.dumps(j))
except Exception as e:
    print('Request failed:', e)
    print('Status:', getattr(r, 'status_code', 'n/a'))
    print('Text:', getattr(r, 'text', '')[:300])
    raise SystemExit(1)
