import glob
import pathlib
import jsonlines

def convert(filename):

    cwd = pathlib.Path().absolute()
    path = pathlib.Path(filename)
    stem = path.stem
    parent = path.parent

    input_fullpath = cwd / path

    output_filename = stem + '.monpoly.log'
    output_parent = cwd / parent
    output_fullpath = output_parent / output_filename

    output_parent.mkdir(parents=True, exist_ok=True)

    outfile = open(output_fullpath , "w")

    # Opening JSON Lines file
    with jsonlines.open(input_fullpath) as reader:

        time = 0

        for obj in reader:
            time = obj['time']
            string = ''
            if 'p' in obj and obj['p'] == True:
                string += ' p'
            if 'q' in obj and obj['q'] == True:
                string += ' q'
            if 'r' in obj and obj['r'] == True:
                string += ' r'
            if 's' in obj and obj['s'] == True:
                string += ' s'

            outfile.write("@{}{}\n".format(time, string))

if __name__ == "__main__":

    traces = glob.glob('**/*.jsonl', recursive=True)

    for trace in  glob.glob('**/*.jsonl', recursive=True):
        convert(trace)
