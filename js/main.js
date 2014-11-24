require(["globe"], function(globe) {
    var earth = globe.Globe();
    globe.animate();
    earth.addStrike(50, 0);
});
