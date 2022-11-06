import os

if __name__ == '__main__':
    output = open("./output", 'w')
    content = []

    for root, folder, files in os.walk('.'):
        root = root[2:]
        for file in files:
            if '.png' in file:
                path = os.path.join(root, file)
                if '(' in file:
                    file = file[:file.find('(') - 1]
                else:
                    file = file[:file.find('.')]
                tag = os.path.join(root, file)
                content.append(tag.replace('/', '_').replace(' ', '_').replace('-', '_') + ": " + path)

    content.sort()
    output.write('\n'.join(content))
