import sys
if sys.version_info < (3, 0):
	sys.stdout.write("Bootstrap requires Python 3.x.\n")
	sys.exit(1)

import argparse
from datetime import date
from html.parser import HTMLParser
import os.path
import re
import subprocess
import urllib.request

import build.indent_print as indent_print
import tools.converter.libs.utf8 as utf8

ProgramOptions = None

def RunCommand(args, expected = 0, vcVars = False):
	if vcVars:
		vcvars_path = os.path.abspath(os.getenv('VS140COMNTOOLS') + '..\\..\\VC\\vcvarsall.bat')
		args = [ vcvars_path, 'amd64', '&&' ] + args

	with subprocess.Popen(args, shell = True, stdout = subprocess.PIPE, stderr = subprocess.STDOUT) as proc:
		for line in iter(proc.stdout.readline, b''):
			print('\t' + str(line.rstrip()))
		proc.communicate()
		print('')
		return proc.returncode == expected

def GenerateSolution():
	print('Generating solution with GYP...')

	if not RunCommand(['tools\gyp\gyp', '--depth=.', 'utf8rewind.gyp']):
		return False

	print('SUCCEEDED')

	return True

def UnicodeToHexadecimal(text):
	codepoints = []

	match = re.findall('([0-9A-Fa-f]+)', text)
	if match:
		for m in match:
			codepoints.append(int(m, 16))
	else:
		print('Input is not hexadecimal.')
		exit(1)
	
	input = 'U+' + format(codepoints[0], '04X')
	for c in codepoints[1:]:
		input += ' U+' + format(c, '04X')

	print(input + ' "' + utf8.unicodeToUtf8(codepoints) + '"')

def DownloadUnicodeData():
	class LinkScraper(HTMLParser):
		def __init__(self, url):
			HTMLParser.__init__(self)

			self.url = url
			self.in_link = False
			self.find_date = re.compile('.*(\d{2})-(\w+)-(\d{4}) (\d{2}):(\d{2})')
			self.months = {
				'Jan': 1,
				'Feb': 2,
				'Mar': 3,
				'Apr': 4,
				'May': 5,
				'Jun': 6,
				'Jul': 7,
				'Aug': 8,
				'Sep': 9,
				'Oct': 10,
				'Nov': 11,
				'Dec': 12,
			}
			self.entries = []

		def __enter__(self):
			self.scrape()

		def __exit__(self, exc_type, exc_val, exc_tb):
			pass

		def scrape(self):
			with urllib.request.urlopen(self.url) as response:
				self.feed(str(response.read()))

		def handle_starttag(self, tag, attrs):
			self.in_link = tag == 'a'
			if self.in_link:
				self.current = {
					'date': '',
					'link': self.url + attrs[0][1]
				}

		def handle_data(self, data):
			if self.in_link:
				match = self.find_date.match(data)
				if match:
					day = int(match.group(1))
					month = self.months[match.group(2)]
					year = int(match.group(3))
					hour = int(match.group(4))
					minute = int(match.group(5))

					self.current['date'] = '%04d-%02d-%02dT%02d:%02d:00Z' % (year, month, day, hour, minute)
					self.entries.append(self.current)

	scraper = LinkScraper('http://www.unicode.org/Public/UCA/')
	scraper.scrape()
	print(scraper.entries)

if __name__ == '__main__':
	parser = argparse.ArgumentParser(description='Bootstraps project and dependencies')
	parser.add_argument(
		'--uni2hex',
		dest = 'uni2hex',
		nargs = '?',
		help = 'convert Unicode code points to hexadecimal UTF-8 encoded string'
	)
	parser.add_argument(
		'--download',
		dest = 'download',
		action = 'store_true',
		help = 'download Unicode data'
	)
	ProgramOptions = parser.parse_args()

	if ProgramOptions.uni2hex:
		UnicodeToHexadecimal(ProgramOptions.uni2hex)
		exit(0)

	if ProgramOptions.download:
		DownloadUnicodeData()
		exit(0)

	with indent_print.guard():
		if not GenerateSolution():
			exit(1)

	exit(0)