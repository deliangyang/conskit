--TEST--
Check for haystack
--SKIPIF--
<?php if (!extension_loaded("haystack")) print "skip"; ?>
--FILE--
<?php 
$parameter = new Haystack\Task\Parameter(['user_id' => 11221827]);
var_dump($parameter);
var_dump($parameter->get('user_id'));
var_dump($parameter->get('does_not_exists_key'));
print_r($parameter->toArray());
var_dump($parameter->set('say', '你好'));
var_dump($parameter->get('say'));
$parameter->set('found', true);
var_dump($parameter->get('found'));
$parameter->set('found', false);
var_dump($parameter->get('found'));
$parameter->set('pi', 3.1415);
var_dump($parameter->get('pi'));
?>
--EXPECT--
object(Haystack\Task\Parameter)#1 (1) {
  ["values":protected]=>
  array(1) {
    ["user_id"]=>
    int(11221827)
  }
}
int(11221827)
NULL
Array
(
    [user_id] => 11221827
)
bool(true)
string(6) "你好"
bool(true)
bool(false)
float(3.1415)