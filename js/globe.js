require.config({
    shim: {
        "THREE": {
            exports: "THREE"
        }
    },
    paths: {
        THREE: "lib/three.min"
    }
});

define([
    "THREE", "text!globe/vertex.c", "text!globe/fragment.c"],
    function(THREE, vertex, fragment) {
        return {
            Globe: function() {
                // TODO:
                //  - Determine multi-light rendering options.
                //  - Don't render until resources all loaded. DONE?

                // Rendering options:
                //  - Deferred rendering
                //  - Overlay blending
                //  - Re-render blending
                //  - MAX_LIGHTS + loop (limited)

                // Initialise the Globe
                var scene = new THREE.Scene();
                var renderer = new THREE.WebGLRenderer({antialias: true});
                renderer.setSize(window.innerWidth, window.innerHeight);
                document.body.appendChild(renderer.domElement);

                utils = THREE.ImageUtils;
                var day = utils.loadTexture('media/day.jpg');
                var night = utils.loadTexture('media/night.jpg');
                var normal = utils.loadTexture('media/normal.png');
                var specular = utils.loadTexture('media/specular.png');

                var material = new THREE.ShaderMaterial({
                    attributes: {},
                    uniforms: {
                        s_day: {type: 't', value: day},
                        s_night: {type: 't', value: night},
                        s_normal: {type: 't', value: normal},
                        s_specular: {type: 't', value: specular},
                        //light_position: {type: '3f', value: [1.0, 2.0, 0.0]}
                        light_position: {type: '3f', value: [0.0, 0.0, 1.1]},
                        light_intensity: {type: 'f', value: 1.0}
                    },
                    vertexShader: vertex,
                    fragmentShader: fragment
                });

                var radius = 1.0;
                var geometry = new THREE.SphereGeometry(radius, 80, 60);
                geometry.computeTangents();
                var globe = new THREE.Mesh(geometry, material);
                scene.add(globe);

                var camera = new THREE.PerspectiveCamera(
                    75, window.innerWidth / window.innerHeight, 0.1, 10);
                camera.position.z = 2.6;
                camera.lookAt(scene.position);

                var clock = new THREE.Clock();

                function animate() {
                    // Time since last frame / seconds
                    var delta = clock.getDelta();
                    //orbitControls.update(delta);
                    globe.rotation.y += delta / 4;//0;

                    requestAnimationFrame(animate);
                    render();
                }

                function render() {
                    renderer.render(scene, camera);
                }

                this.addStrike = function(lat, lon) {
                    console.log('Strike at', lat, lon);
                }

                this.animate = animate;

                return this;
            }
        }
    }
);
