import argparse
import os
import sys
import json

import pylab as pl
import numpy as np

def main(args):
    for solution_file in args.solution_files:
        with open(solution_file, 'r') as f:
            print(solution_file)
            data = json.load(f)
            t0 = data['t_0']
            n_steps = data['n_steps']
            step = data['step']
            x_data = np.array(data['solutions'])
            n_impls = x_data.shape[0]
            timestamps = np.arange(t0, t0 + n_steps*step, step)
            assert x_data.shape[1] == timestamps.shape[0]
            pl.subplot()

            if args.average:
                pl.plot(timestamps, np.average(x_data, axis=0), label='Averaged ' + solution_file)
            else:
                for i in range(n_impls):
                    pl.plot(timestamps, x_data[i], label='impl #{}'.format(i))

            pl.xlabel('t')
            pl.ylabel('X(t)')
            pl.legend(loc='best')

    pl.show()


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='')
    parser.add_argument('solution_files', type=str, nargs='+')
    parser.add_argument('--average', action='store_true')

    main(parser.parse_args())
