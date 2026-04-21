#!/usr/bin/env python3
import argparse, os, requests, json, sys

ap = argparse.ArgumentParser()
ap.add_argument('--token', default=os.environ.get('ACMOJ_TOKEN'))
ap.add_argument('--problem-id', type=int, required=True)
ap.add_argument('--language', type=str, required=True)
ap.add_argument('--code-file', type=str, required=True)
args = ap.parse_args()

tok = args.token
if not tok:
    print('Error: token missing', file=sys.stderr)
    sys.exit(1)

with open(args.code_file, 'r') as f:
    code = f.read()

url = 'https://acm.sjtu.edu.cn/OnlineJudge/api/v1/problem/{}/submit'.format(args.problem_id)
headers = {
    'Authorization': 'Bearer {}'.format(tok),
    'Content-Type': 'application/x-www-form-urlencoded',
    'User-Agent': 'ACMOJ-Python-Client/2.2'
}

data = {
    'language': args.language,
    'code': code
}

r = requests.post(url, headers=headers, data=data, timeout=15, proxies={'https': None, 'http': None})
try:
    r.raise_for_status()
    resp = r.json()
except Exception as e:
    print('Request failed:', e)
    print('Status:', getattr(r, 'status_code', 'n/a'))
    print('Text:', getattr(r, 'text', '')[:500])
    sys.exit(1)
print(json.dumps(resp))
