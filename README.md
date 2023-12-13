# htmle2html

Simple program to allow includes in html files. Work only with *.htmle extension file


```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>
    
    <?e include("header.html") ?>
    <?e include("main.html") ?>
    <?e include("footer.html") ?>

</body>
</html>
```

## Compile examples

```bash
htmle2html examples/htmle out examples/html