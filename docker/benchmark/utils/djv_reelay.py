import glob
import pathlib
import json

def convert(filename):

    cwd = pathlib.Path().absolute()
    path = pathlib.Path(filename)
    stem = path.stem
    parent = path.parent

    input_fullpath = cwd / path

    output_filename = stem + '.jsonl'
    output_parent = cwd / parent
    output_fullpath = output_parent / output_filename

    output_parent.mkdir(parents=True, exist_ok=True)

    outfile = open(output_fullpath , "w")

    is_timed = stem.endswith('timed')

    with open(filename, 'r') as f:
        for line in f:
            count = 1
            args = line.strip().split(',')
            if is_timed:
                # print(json.dumps({'data': args[:-1], 'time': args[-1]}), file=outfile)
                # print(json.dumps({args[0]: args[1:-1], 'time': args[-1]}), file=outfile)
                o = {'event': args[0], 'time': args[-1]}
                for i, arg in enumerate(args[1:-1], 1):
                    o['{}_{}'.format(args[0],i)] = arg
                print(json.dumps(o), file=outfile)

            else:
                # print(json.dumps({'data': args}), file=outfile)
                # print(json.dumps({'event': args[0], 'args': args[1:]}), file=outfile)
                o = {'event': args[0], 'time': count}
                for i, arg in enumerate(args[1:], 1):
                    o['{}_{}'.format(args[0],i)] = arg
                print(json.dumps(o), file=outfile)

if __name__ == "__main__":

    traces = glob.glob('**/*.csv', recursive=True)

    for trace in traces:
        convert(trace)
