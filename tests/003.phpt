--TEST--
Check for haystack
--SKIPIF--
<?php if (!extension_loaded("haystack")) print "skip"; ?>
--FILE--
<?php
class Hello extends Haystack\Task {
    public function execute($p) {}
}
var_dump((new Hello())->getName());
var_dump((new World())->getName());
?>
--EXPECT--
string(5) "Hello"
string(5) "World"