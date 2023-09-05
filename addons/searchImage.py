# -*- coding: UTF-8 -*-

from saucenao_api import SauceNao
import argparse

sauce = SauceNao('')

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--url', type=str, default=None)
    args = parser.parse_args()
    res = sauce.from_url(args.url).results
    print('[', end="")
    for i in range(0, 6):
        print('{', end='')
        print('\"title\":', end='\"')
        print(res[i].title, end='\",')
        print('\"similarity\":', end='\"')
        print(res[i].similarity, end='\",')
        if res[i].urls:
            print('\"url\":', end='\"')
            print(res[i].urls[0], end='\",')
        print('\"index_id\":', end='')
        print(res[i].index_id, end=',')
        print('\"index_name\":', end='\"')
        print(res[i].index_name, end='\",')
        print('\"thumbnail\":', end='\"')
        print(res[i].thumbnail, end='\"}')
        if i != 5:
            print(',', end='')
    print(']', end='')
