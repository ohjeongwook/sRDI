import sys
sys.path.append(r'..\Python')
from ShellcodeRDI import *

input_filename=sys.argv[1]
output_filename=sys.argv[2]

print('Reading', input_filename)
dll=open(input_filename, 'rb').read()
shellcode=ConvertToShellcode(dll)

print('Writing', output_filename)
fd=open(output_filename,'wb')
fd.write(shellcode)
fd.close()
