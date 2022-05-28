
from asyncio.subprocess import PIPE
from random import random
from re import sub
import PySimpleGUI as sg
import os
import subprocess

layout = [[sg.Text('Code'), sg.Multiline('', size=(45, 5), expand_x=False,
                                         expand_y=True, k='-code-'), sg.Text('quadraples'), sg.Multiline('', size=(45, 5), expand_x=False,
                                                                                                         expand_y=True, k='-quads-', disabled=True), sg.Text('symboltable'), sg.Multiline('', size=(45, 5), expand_x=False,
                                                                                                                                                                                          expand_y=True, k='-symbol-', disabled=True), sg.Text('error'), sg.Multiline('', size=(45, 5), expand_x=False,
                                                                                                                                                                                                                                                                      expand_y=True, k='-error-', disabled=True)], [sg.Button("compile")]]

# Create the window
window = sg.Window("Useless-GUI", layout, finalize=True)
window.set_min_size((800, 300))
# Create an event loop


def readfile(filename):
    s = ""
    f = open(filename)
    for line in f.readlines():
        s += line
    return s


def writetofile(s, filename):
    f = open(filename, "w")
    f.writelines(s)


def execcompiler():
    print("start compiling")
    #print(os.system("./app.out < ../outputs/code.ul"))
    p = subprocess.Popen("./app.out", stdin=open("../outputs/code.ul"),
                         stdout=PIPE)
    # open("../../outputs/output_compiler", "w")
    p.wait()
    print(p.stdout.read())
    # p = subprocess.run(["../../build/app.out"], stdin=open("../../outputs/code.ul"),
    #                    capture_output=True)
    print("executed")


while True:
    event, values = window.read()
    # window['-quads-'].update(default_text=str(random()))

    if event == "compile":
        writetofile(window['-code-'].Get(), "../outputs/code.ul")
        execcompiler()
        window['-quads-'].update(readfile("../outputs/quads.asm"))
        window['-error-'].update(readfile("../outputs/error.err"))
        window['-symbol-'].update(readfile("../outputs/symboltable.st"))
        # print(window['-quads-'].DefaultText)
    if event == sg.WIN_CLOSED:
        break

window.close()
