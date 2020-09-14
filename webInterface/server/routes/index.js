const express = require("express");
const router = express.Router();

const path = require("path");


router.use(express.static(path.join(__dirname, '../../map_test/build')));

router.get('*', function (req, res) {
  res.sendFile(path.join(__dirname, '../../map_test/build', 'index.html'));
});


module.exports = router;


