#include "commandhelper.h"
#include <QVBoxLayout>
#include <QTextEdit>

CommandHelper::CommandHelper(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    setWindowTitle("Shell Command Reference");
    resize(900, 700);
}

CommandHelper::~CommandHelper() {
}

void CommandHelper::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    tabWidget_ = new QTabWidget(this);

    // Create help text edits
    sedHelp_ = new QTextEdit(this);
    sedHelp_->setReadOnly(true);
    addSedExamples();

    awkHelp_ = new QTextEdit(this);
    awkHelp_->setReadOnly(true);
    addAwkExamples();

    grepHelp_ = new QTextEdit(this);
    grepHelp_->setReadOnly(true);
    addGrepExamples();

    cutHelp_ = new QTextEdit(this);
    cutHelp_->setReadOnly(true);
    addCutExamples();

    generalHelp_ = new QTextEdit(this);
    generalHelp_->setReadOnly(true);
    addGeneralExamples();

    // Add tabs
    tabWidget_->addTab(generalHelp_, "General");
    tabWidget_->addTab(grepHelp_, "grep");
    tabWidget_->addTab(sedHelp_, "sed");
    tabWidget_->addTab(awkHelp_, "awk");
    tabWidget_->addTab(cutHelp_, "cut");

    mainLayout->addWidget(tabWidget_);

    // Style
    QString style = R"(
        QTextEdit {
            background-color: #1e1e1e;
            color: #d4d4d4;
            font-family: 'Consolas', 'Monaco', monospace;
            font-size: 12px;
        }
    )";
    setStyleSheet(style);
}

void CommandHelper::addSedExamples() {
    sedHelp_->setMarkdown(R"(
# sed - Stream Editor

## Basic Syntax
```bash
sed [options] 'command' file
```

## Common Options
- `-i` - Edit files in-place
- `-n` - Suppress automatic printing
- `-e` - Multiple commands
- `-r` or `-E` - Extended regex

## Substitution (s command)

### Replace first occurrence
```bash
sed 's/old/new/' file
```

### Replace all occurrences (global)
```bash
sed 's/old/new/g' file
```

### Replace on specific line
```bash
sed '3s/old/new/' file
```

### Case insensitive replace
```bash
sed 's/old/new/gi' file
```

## Deletion (d command)

### Delete line 3
```bash
sed '3d' file
```

### Delete lines 2-5
```bash
sed '2,5d' file
```

### Delete lines matching pattern
```bash
sed '/pattern/d' file
```

### Delete empty lines
```bash
sed '/^$/d' file
```

## Print (p command)

### Print only line 5
```bash
sed -n '5p' file
```

### Print lines 10-20
```bash
sed -n '10,20p' file
```

### Print lines matching pattern
```bash
sed -n '/pattern/p' file
```

## Insertion & Appending

### Insert line before line 3
```bash
sed '3i\New line' file
```

### Append line after line 3
```bash
sed '3a\New line' file
```

## Advanced Examples

### Multiple substitutions
```bash
sed -e 's/foo/bar/g' -e 's/old/new/g' file
```

### Backreferences
```bash
sed 's/\([0-9]*\)/Number: \1/' file
```

### In-place edit with backup
```bash
sed -i.bak 's/old/new/g' file
```
)");
}

void CommandHelper::addAwkExamples() {
    awkHelp_->setMarkdown(R"(
# awk - Pattern Scanning and Processing

## Basic Syntax
```bash
awk 'pattern { action }' file
```

## Built-in Variables
- `$0` - Entire line
- `$1, $2, ...` - Field 1, field 2, etc.
- `NF` - Number of fields
- `NR` - Current line number
- `FS` - Field separator (default: whitespace)
- `OFS` - Output field separator

## Print Commands

### Print entire file
```bash
awk '{print}' file
```

### Print specific field
```bash
awk '{print $1}' file
```

### Print multiple fields
```bash
awk '{print $1, $3}' file
```

### Print with custom separator
```bash
awk '{print $1 ":" $2}' file
```

## Pattern Matching

### Print lines containing pattern
```bash
awk '/pattern/' file
```

### Print lines NOT containing pattern
```bash
awk '!/pattern/' file
```

### Print if field matches
```bash
awk '$3 == "value"' file
```

## Field Separator

### Custom field separator
```bash
awk -F: '{print $1}' /etc/passwd
```

### Multiple separators
```bash
awk -F'[,:]' '{print $1}' file
```

## Calculations

### Sum a column
```bash
awk '{sum+=$3} END {print sum}' file
```

### Average
```bash
awk '{sum+=$1} END {print sum/NR}' file
```

### Count lines
```bash
awk 'END {print NR}' file
```

## Conditional Statements

### If statement
```bash
awk '{if ($3 > 100) print $1}' file
```

### If-else
```bash
awk '{if ($3 > 100) print "high"; else print "low"}' file
```

## Advanced Examples

### Print line numbers
```bash
awk '{print NR, $0}' file
```

### Print last field
```bash
awk '{print $NF}' file
```

### Print lines longer than 80 chars
```bash
awk 'length($0) > 80' file
```

### Format output
```bash
awk '{printf "%-10s %5d\n", $1, $2}' file
```

### Remove duplicates
```bash
awk '!seen[$0]++' file
```
)");
}

void CommandHelper::addGrepExamples() {
    grepHelp_->setMarkdown(R"(
# grep - Global Regular Expression Print

## Basic Syntax
```bash
grep [options] pattern [file...]
```

## Common Options
- `-i` - Case insensitive
- `-v` - Invert match (show non-matching)
- `-n` - Show line numbers
- `-c` - Count matches
- `-l` - List filenames only
- `-r` or `-R` - Recursive search
- `-E` - Extended regex (or use `egrep`)
- `-w` - Match whole words
- `-A n` - Show n lines after match
- `-B n` - Show n lines before match
- `-C n` - Show n lines before and after

## Basic Searches

### Simple search
```bash
grep "pattern" file
```

### Case insensitive
```bash
grep -i "pattern" file
```

### Whole word
```bash
grep -w "word" file
```

### Multiple files
```bash
grep "pattern" file1 file2 file3
```

## Regular Expressions

### Match start of line
```bash
grep "^pattern" file
```

### Match end of line
```bash
grep "pattern$" file
```

### Match either pattern
```bash
grep -E "pattern1|pattern2" file
```

### Match digit
```bash
grep "[0-9]" file
```

### Match email
```bash
grep -E "[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}" file
```

## Context & Formatting

### Show line numbers
```bash
grep -n "pattern" file
```

### Show 3 lines after match
```bash
grep -A 3 "pattern" file
```

### Show 3 lines before match
```bash
grep -B 3 "pattern" file
```

### Show 3 lines before and after
```bash
grep -C 3 "pattern" file
```

## Counting & Listing

### Count matches
```bash
grep -c "pattern" file
```

### List files containing pattern
```bash
grep -l "pattern" *.txt
```

### List files NOT containing pattern
```bash
grep -L "pattern" *.txt
```

## Recursive Search

### Search in all files recursively
```bash
grep -r "pattern" /path/to/dir
```

### Recursive with line numbers
```bash
grep -rn "pattern" .
```

### Recursive for specific file type
```bash
grep -r --include="*.js" "pattern" .
```

## Invert Match

### Show lines NOT matching
```bash
grep -v "pattern" file
```

### Remove empty lines
```bash
grep -v "^$" file
```

### Remove comments
```bash
grep -v "^#" file
```

## Advanced Examples

### Multiple patterns (AND)
```bash
grep "pattern1" file | grep "pattern2"
```

### Multiple patterns (OR)
```bash
grep -E "pattern1|pattern2" file
```

### Highlight matches
```bash
grep --color=auto "pattern" file
```

### Search binary files as text
```bash
grep -a "pattern" binaryfile
```
)");
}

void CommandHelper::addCutExamples() {
    cutHelp_->setMarkdown(R"(
# cut - Remove Sections from Lines

## Basic Syntax
```bash
cut [options] [file...]
```

## Common Options
- `-c` - Select by character position
- `-f` - Select by field
- `-d` - Field delimiter (default: TAB)
- `--complement` - Invert selection
- `-s` - Suppress lines with no delimiter
- `--output-delimiter` - Specify output delimiter

## Character-based Selection

### Extract characters 1-5
```bash
cut -c1-5 file
```

### Extract character 10 onward
```bash
cut -c10- file
```

### Extract multiple character ranges
```bash
cut -c1-3,10-15 file
```

### Extract specific characters
```bash
cut -c1,5,9 file
```

## Field-based Selection

### Extract first field (TAB-delimited)
```bash
cut -f1 file
```

### Extract fields 1 and 3
```bash
cut -f1,3 file
```

### Extract field range
```bash
cut -f2-5 file
```

### Extract from field 3 onward
```bash
cut -f3- file
```

## Custom Delimiter

### Colon-delimited (e.g., /etc/passwd)
```bash
cut -d: -f1 /etc/passwd
```

### Comma-delimited (CSV)
```bash
cut -d, -f2,4 file.csv
```

### Space-delimited
```bash
cut -d' ' -f1 file
```

## Advanced Examples

### Extract usernames from /etc/passwd
```bash
cut -d: -f1 /etc/passwd
```

### Extract IP addresses (assuming space-delimited)
```bash
cut -d' ' -f1 access.log
```

### CSV: Extract columns 2 and 3
```bash
cut -d, -f2,3 data.csv
```

### Custom output delimiter
```bash
cut -d: -f1,3 --output-delimiter=" - " /etc/passwd
```

### Invert selection (all except field 2)
```bash
cut -d, -f2 --complement file.csv
```

### Suppress lines without delimiter
```bash
cut -d: -f1 -s file
```

## Combining with Other Commands

### Extract filenames from ls -l
```bash
ls -l | cut -d' ' -f9-
```

### Get running process names
```bash
ps aux | cut -c66-
```

### Extract domain from URLs
```bash
cat urls.txt | cut -d/ -f3
```

### First column, sorted, unique
```bash
cut -d, -f1 data.csv | sort | uniq
```
)");
}

void CommandHelper::addGeneralExamples() {
    generalHelp_->setMarkdown(R"DELIM(
# Shell Command Cheat Sheet

## Pipes and Redirection

### Pipe output to another command
```bash
command1 | command2
```

### Redirect output to file (overwrite)
```bash
command > file
```

### Redirect output to file (append)
```bash
command >> file
```

### Redirect stderr to stdout
```bash
command 2>&1
```

### Redirect both stdout and stderr
```bash
command &> file
```

## Combining Commands

### Run sequentially (regardless of success)
```bash
command1 ; command2
```

### Run if previous succeeded (AND)
```bash
command1 && command2
```

### Run if previous failed (OR)
```bash
command1 || command2
```

## Common Combinations

### Find and grep
```bash
find . -name "*.txt" | xargs grep "pattern"
```

### Sort and unique
```bash
cat file | sort | uniq
```

### Count unique lines
```bash
sort file | uniq -c | sort -nr
```

### Head and tail
```bash
head -n 20 file | tail -n 10
```

## Text Processing Pipeline

### Extract, sort, count
```bash
cut -d, -f1 data.csv | sort | uniq -c
```

### Filter, transform, aggregate
```bash
grep "ERROR" log.txt | awk '{print $3}' | sort | uniq -c | sort -nr
```

### Multi-stage processing
```bash
cat file | grep "pattern" | sed 's/old/new/g' | awk '{print $1}' | sort | uniq
```

## Useful Utilities

### `wc` - Word count
```bash
wc -l file      # Count lines
wc -w file      # Count words
wc -c file      # Count bytes
```

### `sort` - Sort lines
```bash
sort file       # Alphabetical sort
sort -n file    # Numerical sort
sort -r file    # Reverse sort
sort -u file    # Unique sorted
```

### `uniq` - Remove duplicates
```bash
uniq file       # Remove consecutive duplicates
uniq -c file    # Count occurrences
uniq -d file    # Show only duplicates
```

### `tr` - Translate characters
```bash
tr 'a-z' 'A-Z'  # Uppercase
tr -d '\n'      # Delete newlines
tr -s ' '       # Squeeze spaces
```

## Advanced Patterns

### Process substitution
```bash
diff <(sort file1) <(sort file2)
```

### Command substitution
```bash
echo "Today is $(date)"
```

### Xargs for parallel processing
```bash
find . -name "*.txt" | xargs -P 4 -I {} process {}
```

### Here document
```bash
cat << EOF
Multiple lines
of text
EOF
```
)DELIM");
}
