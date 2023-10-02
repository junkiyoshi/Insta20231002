#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofSetColor(0);
	ofEnableDepthTest();

	auto ico_sphere = ofIcoSpherePrimitive(300, 5);
	this->triangle_list.insert(this->triangle_list.end(), ico_sphere.getMesh().getUniqueFaces().begin(), ico_sphere.getMesh().getUniqueFaces().end());
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->mesh.clear();
	for (auto radius = 150; radius <= 250; radius += 1) {

		for (int k = 0; k < this->triangle_list.size(); k++) {

			glm::vec3 avg = (this->triangle_list[k].getVertex(0) + this->triangle_list[k].getVertex(1) + this->triangle_list[k].getVertex(2)) / 3;

			if (ofNoise(glm::vec4(avg * 0.008, ofGetFrameNum() * 0.01)) < 0.5) { continue; }

			vector<glm::vec3> vertices;
			vertices.push_back(glm::normalize(this->triangle_list[k].getVertex(0)) * radius);
			vertices.push_back(glm::normalize(this->triangle_list[k].getVertex(1)) * radius);
			vertices.push_back(glm::normalize(this->triangle_list[k].getVertex(2)) * radius);

			this->mesh.addVertices(vertices);

			for (int i = 0; i < 3; i++) {

				auto noise_value = ofNoise(glm::vec4(this->triangle_list[k].getVertex(i) * 0.008, ofGetFrameNum() * 0.01));
				this->mesh.addColor(ofColor(ofMap(radius, 150, 250, 0, 255), noise_value > 0.55 ? 255 : ofMap(noise_value, 0.5, 0.55, 0, 255)));
			}

			this->mesh.addTriangle(this->mesh.getNumVertices() - 1, this->mesh.getNumVertices() - 2, this->mesh.getNumVertices() - 3);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum());

	this->mesh.draw();

	ofDrawSphere(150);

	this->cam.end();

	int start = 1;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/today/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}