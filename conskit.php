<?php

class Add {
    public function execute(Conskit\Parameter $p) {
        $a = $p->get('a') ?: 0;
        $b = $p->get('b') ?: 0;
        echo $a + $b;
    }
}

class StatRetention {
    public function execute(Conskit\Parameter $p) {
       
    }
}


$app = new Conskit\Application();
$app->start();