A tool to help the *incompetent* free software enthusiast code review open source libraries.

Because, let's be honest, you're already doing this manually, but why waste time when a script can automate your "expert analysis"?

---

### FOSS Enthusiast Workflow:
1: Some indie developer posts a lib they spent weeks or months designing on reddit/dayle.dev, thinking someone might appreciate their hard work.<br>
2: The FOSS Enthusiast (you) heroically steps in, opens the lib’s GitHub at a **completely random** file, with **zero context**, **zero understanding**, and **infinite confidence**, and proceeds to drop the most uninformed, condescending "feedback" imaginable.

## avgfosshelper workflow:
Now, with `avgfosshelper`, you can automate your rigorous review process:

```bash
avgfosshelper <github-repo-url>
```
It will display **random** files with **random** chunks of code from that file, on repeat, until you feel sufficiently informed to write a blog post explaining why the project is garbage.

---

## Building
Of course, this project is built with **CMake**, because it's the only build system I can safely assume you have installed. If you're one of those "Rust is the future" people—too bad.

To compile, just run these commands (no need to understand what they do, you never do anyway):

```bash
git clone -b 29.5.125 https://github.com/mateusmoutinho/avgfosshelper &&
cd avgfosshelper &&
mkdir build &&
cd build &&
cmake .. &&
make &&
sudo mv main.out /usr/bin/avgfosshelper
```

---

## CLI Flags
Yes, flags. Because every serious tool needs flags, even if you won’t use them.

| Flag         | Description                                 | Default        |
|-------------|---------------------------------|-------------|
| --cache     | Where to store the cache, because why not? | /tmp/cache  |
| --timeout   | Time before re-fetching the URL, because your attention span is short. | 300s |
| --line-chunk | The size of the code chunk to randomly display, just enough to miss all context. | 40 lines |

---

## Features
- Contains a **typo** in the name, for extra authenticity.
- Variable names **max 4 chars**, because readability is a myth.
- **Comments everywhere**, especially where they’re **painfully obvious**.
- **Low file count**, because big projects are for suckers.
- **Minimum 200 lines per file**, because scrolling builds character.
- **All function names are cryptic abbreviations**, just like you would have written them.
- **Does not work for C++**, because I don't care.
- Uses a **highly advanced mathematical model** to determine which file you were most likely to open first, ensuring maximum irrelevance.

---

Enjoy your effortless critique. You're welcome.

