<?php

$data = file_get_contents('php://stdin');

echo "const static char bootloader[] = {\n   ";
for ($i=0; $i<strlen($data); $i++) {
    printf("0x%02x", ord($data[$i]));
    $last = $i+1 == strlen($data);
    if (!$last) {
        echo ", " ;
        if ($i%10 == 9) echo "\n   ";
    } else {
        echo "\n";
    }
}
echo "};\n";
