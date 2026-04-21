#!/usr/bin/env python3
import argparse, os, requests, json

parser = argparse.ArgumentParser()
parser.add_argument('--token', default=os.environ.get('ACMOJ_TOKEN'))
parser.add_argument('--problem-id', type=int, required=True)
parser.add_argument('--language', type=str, required=True)
parser.add_argument('--code-file', type=str, required=True)
args = parser.parse_args()

if not args.token:
    print('Error: token missing')
    raise SystemExit(1)

with open(args.code-file, 'r') as f:
    code = f.read()

url = fhttps://acm.sjtu.edu.cn/OnlineJudge/api/v1/problem/{args.problem_id}/submit
headers = { 'Authorization': f'Bearer {args.token}', 'Content-Type': 'application/x-www-form-urlencoded', 'User-Agent': 'ACMOJ-Python-Client/2.2' }

data = { 'language': args.language, 'code': code }

r = requests.post(url, headers=headers, data=data, timeout=15, proxies={'https': None, 'http': None})
try:
    j = r.json()
except Exception:
    print('Non-JSON response:', r.status_code, r.text[:200])
    raise SystemExit(1)
print(json.dumps(j))
