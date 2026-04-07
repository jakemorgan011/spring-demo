// need width height
// need

#include "ofMain.h"
#include "ofxBox2d.h"
/* ---------------------------------------------------------------------------------------------

 if you don't have ofxBox2d.h go here and clone it inside your addons folder
 https://github.com/ofWorks/ofxBox2d.git

// --------------------------------------------------------------------------------------------- */
#include <stdio.h>
#include <map>
#include <string>
#include <unordered_map>

namespace termite {

/*
   __                       __      __
  / /____  ____ ___  ____  / /___ _/ /____
 / __/ _ \/ __ `__ \/ __ \/ / __ `/ __/ _ \
/ /_/  __/ / / / / / /_/ / / /_/ / /_/  __/
\__/\___/_/ /_/ /_/ .___/_/\__,_/\__/\___/
               /_/
 */
/// a fancy way to not have to deal with types.
/// type: what a variable is...        ie: int foo; would be an integer.
template <typename type> class d_array{
public:
    // default constructor will make space for double the size of one variable.
    d_array(){
        capacity = 0;
        count = 0;
        items = nullptr;
    }
    // optional secondary constructor for declarations with {}.
    d_array(std::initializer_list<type> init){
        count =  init.size();
        capacity = count;
        items = new type[capacity];
        size_t i = 0;
        for(const type& data : init){
            items[i++] = data;
        }
    }
    // copy
    d_array(const d_array& other){
        count = other.count;
        capacity = other.capacity;
        items = new type[capacity];
        for(size_t i = 0; i < count; ++i){
            items[i] = other.items[i];
        }
    }
    // assignment.
    d_array& operator=(const d_array& other){
        if(this != &other){
            delete[] items;
            count = other.count;
            capacity = other.capacity;
            items = new type[capacity];
            for(size_t i = 0; i < count; ++i){
                items[i] = other.items[i];
            }
        }
        return *this;
    }
    type& operator[](size_t idx){
        if(idx >= count){
            throw std::out_of_range("TERMITE: idx out of range in d_array");
        }
        return items[idx];
    }
    const type& operator[](size_t idx) const {
        if(idx >= count){
            throw std::out_of_range("TERMITE: idx out of range in d_array");
        }
        return items[idx];
    }
    // techincally this makes references... which is fine ... i think.
    void append(const type& data){
        if(count >= capacity){
            capacity = (capacity == 0) ? 1 : capacity * 2;
            type* n_items = new type[capacity];
            
            for(size_t i = 0; i < count; ++i){
                n_items[i] = items[i];
            }
            
            delete[] items;
            items = n_items;
        }
        items[count] = data;
        count++;
    }
    void remove_idx(size_t idx){
        if(idx >= count){
            std::cout << "TERMITE: idx out of range in d_array" << std::endl;
            return;
        }
        // technically this just moves everything down and leaves the memory at the end
        // repeated.
        // it can't be accessed though.
        for(size_t i = idx; i < count - 1; ++i){
            items[i] = items[i + 1];
        }
        count--;
    }
    type get_and_remove_idx(size_t idx){
        if(idx >= count){
            throw std::out_of_range("TERMITE: idx out of range in d_array");
        }
        type output = items[idx];
        remove_idx(idx);
        return output;
    }
    type get_at_idx(size_t idx){
        if(idx >= count){
            throw std::out_of_range("TERMITE: idx out of range in d_array");
        }
        return items[idx];
    }
    void clear(){
        count = 0;
    }
    void full_clear(){
        delete[] items;
        items = nullptr;
        count = 0;
        capacity = 0;
    }
    size_t size() const {
        return count;
    }
    ~d_array() {
        delete[] items;
    }
    d_array* get(){
        return this;
    }
    d_array copy(){
        return *this;
    }
    // be careful with this one.
    // if deleted can have pointers that are empty and will break whole thing.
    type* get_items(){
        return items;
    }
private:
    type* items;
    size_t count;
    size_t capacity;
};

//
/*
         _____    __         ___   ____
  ____  / __/ |  / /__  ____|__ \ / __/
 / __ \/ /_ | | / / _ \/ ___/_/ // /_
/ /_/ / __/ | |/ /  __/ /__/ __// __/
\____/_/    |___/\___/\___/____/_/
 */
///an example of how something like ofVec2f looks in code.
struct tVec2f{
    struct proxy{// exists as a reference to x any why so that we can return xy as a vector instead of using a function call.
        float& x;
        float& y;
        operator tVec2f() const {return {x,y};}
    };
    float x = 0;
    float y = 0;
    proxy xy{x,y};
    float distance(const tVec2f& other){
        return std::sqrt(std::pow(other.x - x,2) + std::pow(other.y - y,2));
    }
    tVec2f& operator=(const tVec2f& other){
        if(this != &other){
            x=other.x;
            y=other.y;
        }
        return *this;
    }
    tVec2f& operator+=(const tVec2f& other){
        x+=other.x;
        y+=other.y;
        return *this;
    }
    tVec2f& operator+=(const float& other){
        x+=other;
        y+=other;
        return *this;
    }
    tVec2f& operator-=(const tVec2f& other){
        x-=other.x;
        y-=other.y;
        return *this;
    }
    tVec2f& operator-=(const float& other){
        x-=other;
        y-=other;
        return *this;
    }
    tVec2f& operator/=(const tVec2f& other){
        x/=other.x;
        y/=other.y;
        return *this;
    }
    tVec2f& operator/=(const float& other){
        x/=other;
        y/=other;
        return *this;
    }
    tVec2f& operator*=(const tVec2f& other){
        x*=other.x;
        y*=other.y;
        return *this;
    }
    tVec2f& operator*=(const float& other){
        x*=other;
        y*=other;
        return *this;
    }
    tVec2f operator + (const tVec2f& other)const{
        return{x + other.x, y + other.y};
    }
    tVec2f operator + (const float& other)const{
        return{x + other, y + other};
    }
    tVec2f operator - (const tVec2f& other)const{
        return{x - other.x, y - other.y};
    }
    tVec2f operator - (const float& other)const{
        return{x - other, y - other};
    }
    tVec2f operator * (const tVec2f& other)const{
        return{x * other.x, y * other.y};
    }
    tVec2f operator * (const float& other)const{
        return{x * other, y * other};
    }
    tVec2f operator / (const tVec2f& other)const{
        return{x / other.x, y / other.y};
    }
    tVec2f operator / (const float& other)const{
        return{x / other, y / other};
    }
};
/*
   ___ _               _
  / _ \ |__  _   _ ___(_) ___ ___
 / /_)/ '_ \| | | / __| |/ __/ __|
/ ___/| | | | |_| \__ \ | (__\__ \
\/    |_| |_|\__, |___/_|\___|___/
           |___/
 */
/// we'll be using ofxBox2d as our physics engine.

class body {
    /// holds a shared Box2D world reference and defines the interface all physics bodies must implement.
    /// think of it like adding a Rigidbody component in Unity — the drawable owns one of these.
public:
    body(std::shared_ptr<ofxBox2d> world) : world(world) {}
    virtual ~body() = default;

    virtual void setup(float cx, float cy, float radius, size_t num_particles) = 0;
    virtual void update() = 0;
    virtual std::vector<ofVec2f> get_positions() = 0; // drawable reads this each frame to build its path

    virtual void apply_force(ofVec2f force) = 0;
    virtual void check_bounds(float w, float h) = 0;
    virtual ofVec2f get_velocity() { return {0.f, 0.f}; }

protected:
    std::shared_ptr<ofxBox2d> world;
};

class soft_body : public body {
    /// a ring of small circle particles connected to neighbors with spring joints.
    ///
    ///   spring_freq  — stiffness. higher = snaps back faster
    ///   spring_damp  — 0 = bounces forever, 1 = settles immediately with no overshoot
public:
    soft_body(std::shared_ptr<ofxBox2d> world, float freq = 10.2f, float damp = 0.02f)
        : body(world), spring_freq(freq), spring_damp(damp) {
        group_index = next_group--; // each instance gets its own unique negative group
    }

    void setup(float cx, float cy, float radius, size_t num_particles) override {
        particles.clear();
        joints.clear();
        spokes.clear();

        // same negative groupIndex = particles in this blob never collide with each other,
        // but WILL collide with particles from other blobs (different group index)
        b2Filter filter;
        filter.groupIndex = group_index;

        for(size_t i = 0; i < num_particles; i++){
            float angle = (TWO_PI / num_particles) * i;
            auto p = std::make_shared<ofxBox2dCircle>();
            p->setPhysics(1.0f, 0.3f, 0.5f); // density, bounce, friction
            p->setup(world->getWorld(), cx + std::cos(angle) * radius, cy + std::sin(angle) * radius, radius * 0.15f);
            p->body->GetFixtureList()->SetFilterData(filter);
            p->body->SetLinearDamping(0.8f); // drag — prevents runaway velocity under force
            particles.push_back(p);
        }

        // center anchor — large enough to fill the ring interior so blobs collide with each other
        center = std::make_shared<ofxBox2dCircle>();
        center->setPhysics(0.5f, 0.3f, 0.5f);
        center->setup(world->getWorld(), cx, cy, radius * 0.55f);
        center->body->GetFixtureList()->SetFilterData(filter);
        center->body->SetLinearDamping(0.8f);

        // rim joints — connect each particle to its neighbor (keeps the skin intact)
        for(size_t i = 0; i < num_particles; i++){
            auto joint = std::make_shared<ofxBox2dJoint>();
            joint->setup(world->getWorld(),
                         particles[i]->body,
                         particles[(i + 1) % num_particles]->body,
                         spring_freq,
                         spring_damp);
            joints.push_back(joint);
        }

        // spoke joints — connect every particle to the center (prevents folding inward)
        for(size_t i = 0; i < num_particles; i++){
            auto spoke = std::make_shared<ofxBox2dJoint>();
            spoke->setup(world->getWorld(),
                         center->body,
                         particles[i]->body,
                         spring_freq,
                         spring_damp);
            spokes.push_back(spoke);
        }
    }

    void update() override {
        // Box2D handles simulation — world->update() must be called once per frame in ofApp::update()
    }

    std::vector<ofVec2f> get_positions() override {
        std::vector<ofVec2f> out;
        for(auto& p : particles) out.push_back(p->getPosition());
        return out;
    }

    void apply_force(ofVec2f force) override {
        b2Vec2 f(force.x, force.y);
        center->body->ApplyForceToCenter(f, true);
        for(auto& p : particles)
            p->body->ApplyForceToCenter(f, true);
    }

    void check_bounds(float w, float h) override {
        auto bounce = [](std::shared_ptr<ofxBox2dCircle>& body, float w, float h){
            ofVec2f pos = body->getPosition();
            b2Vec2 vel = body->body->GetLinearVelocity();
            if(pos.x < 0 || pos.x > w) body->body->SetLinearVelocity(b2Vec2(-vel.x,  vel.y));
            if(pos.y < 0 || pos.y > h) body->body->SetLinearVelocity(b2Vec2( vel.x, -vel.y));
        };
        bounce(center, w, h);
        for(auto& p : particles) bounce(p, w, h);
    }

    ofVec2f get_velocity() override {
        if(!center) return {0.f, 0.f};
        b2Vec2 v = center->body->GetLinearVelocity();
        return {v.x, v.y};
    }

    void set_spring(float freq, float damp){ spring_freq = freq; spring_damp = damp; }

private:
    static int16_t next_group; // counts down: -1, -2, -3... one per blob instance
    int16_t group_index;

    std::shared_ptr<ofxBox2dCircle> center;
    std::vector<std::shared_ptr<ofxBox2dCircle>> particles;
    std::vector<std::shared_ptr<ofxBox2dJoint>> joints;   // rim
    std::vector<std::shared_ptr<ofxBox2dJoint>> spokes;   // center → each particle
    float spring_freq;
    float spring_damp;
};
inline int16_t soft_body::next_group = -1; // start at -1, each new blob decrements

class hard_body : public body {
    /// a single solid Box2D circle — one rigid body, no spring joints.
    /// collides and bounces but never deforms.
public:
    hard_body(std::shared_ptr<ofxBox2d> world) : body(world) {}

    void setup(float cx, float cy, float radius, size_t) override { // num_particles unused
        shape = std::make_shared<ofxBox2dCircle>();
        shape->setPhysics(1.0f, 0.6f, 0.3f); // density, bounce, friction
        shape->setup(world->getWorld(), cx, cy, radius);
    }

    void update() override {}

    std::vector<ofVec2f> get_positions() override {
        return { shape->getPosition() }; // just the center — drawable draws a fixed circle
    }

    void apply_force(ofVec2f force) override {
        shape->body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
    }

    void check_bounds(float w, float h) override {
        ofVec2f pos = shape->getPosition();
        b2Vec2 vel = shape->body->GetLinearVelocity();
        if(pos.x < 0 || pos.x > w) shape->body->SetLinearVelocity(b2Vec2(-vel.x,  vel.y));
        if(pos.y < 0 || pos.y > h) shape->body->SetLinearVelocity(b2Vec2( vel.x, -vel.y));
    }

    ofVec2f get_velocity() override {
        if(!shape) return {0.f, 0.f};
        b2Vec2 v = shape->body->GetLinearVelocity();
        return {v.x, v.y};
    }

private:
    std::shared_ptr<ofxBox2dCircle> shape;
};

class static_body : public body {
    /// a fixed, immovable Box2D circle — zero mass, never moves, never affected by forces or gravity.
    /// other bodies collide with it and bounce off, but it stays exactly where it was placed.
    /// think of it like a bumper or obstacle.
public:
    static_body(std::shared_ptr<ofxBox2d> world) : body(world) {}

    void setup(float cx, float cy, float radius, size_t) override {
        shape = std::make_shared<ofxBox2dCircle>();
        shape->setPhysics(0.f, 0.8f, 0.3f); // density 0 = static in Box2D, high bounce
        shape->setup(world->getWorld(), cx, cy, radius);
        shape->body->SetType(b2_staticBody); // locks it in place — no forces can move it
    }

    void update() override {}

    std::vector<ofVec2f> get_positions() override {
        return { shape->getPosition() };
    }

    void apply_force(ofVec2f) override {}
    void check_bounds(float, float) override {}

private:
    std::shared_ptr<ofxBox2dCircle> shape;
};

class spinning_body : public body {
    /// kinematic polygon — rotates in place, collisions match the spinning shape.
    /// uses ofxBox2dPolygon so coordinate scaling is handled correctly.
public:
    spinning_body(std::shared_ptr<ofxBox2d> world, float degrees_per_frame)
        : body(world) {
        angular_vel = degrees_per_frame * (PI / 180.f) * 60.f; // deg/frame → rad/sec at 60fps
    }

    void setup(float cx, float cy, float radius, size_t num_verts) override {
        shape = std::make_shared<ofxBox2dPolygon>();
        for(size_t i = 0; i < num_verts; i++){
            float a = (TWO_PI / num_verts) * i;
            shape->addVertex(glm::vec3(cx + std::cos(a) * radius, cy + std::sin(a) * radius, 0.f));
        }
        shape->setPhysics(0.f, 0.8f, 0.3f);
        shape->create(world->getWorld());
        shape->body->SetType(b2_kinematicBody);
        shape->body->SetAngularVelocity(angular_vel);
    }

    void update() override {}
    std::vector<ofVec2f> get_positions() override { return {}; }
    void apply_force(ofVec2f) override {}
    void check_bounds(float, float) override {}

    void set_angular_velocity(float degrees_per_frame){
        angular_vel = degrees_per_frame * (PI / 180.f) * 60.f;
        if(shape && shape->body) shape->body->SetAngularVelocity(angular_vel);
    }

private:
    std::shared_ptr<ofxBox2dPolygon> shape;
    float angular_vel = 0.f;
};



//
/*
    ___    __         __                  __     ________
   /   |  / /_  _____/ /__________ ______/ /_   / ____/ /___ ______________  _____
  / /| | / __ \/ ___/ __/ ___/ __ `/ ___/ __/  / /   / / __ `/ ___/ ___/ _ \/ ___/
 / ___ |/ /_/ (__  ) /_/ /  / /_/ / /__/ /_   / /___/ / /_/ (__  |__  )  __(__  )
/_/  |_/_.___/____/\__/_/   \__,_/\___/\__/   \____/_/\__,_/____/____/\___/____/
                                                                               
*/
/// essentially just an ultra modular object.


// this will be our base object so that we can create tons of variations on it and still reference it with a small base class.
struct drawable { // abstract
public:
    virtual void setup(){}
    virtual void draw(){}
    virtual void update(){}
    virtual void init(std::shared_ptr<ofxBox2d> world){}
    virtual void set_pos(const float x, const float y){}
    virtual tVec2f get_pos(){return {0.f,0.f};}
    virtual ~drawable() = default;
    void set_color(int r, int g, int b){
        color = ofColor(r,g,b);
    }
    virtual std::string get_info() {return "";}
    virtual void set_bounds(const float width, const float height){}
    virtual void bind_to_drawable(std::shared_ptr<drawable> d){}
    std::unique_ptr<body> physics;
protected:
    ofColor color = {212,212,212};
private:
};
class mover : public drawable { // abstract
public:
    virtual void apply_force(tVec2f& force){
        tVec2f f = force/mass;
        accel+=f;
    }
    virtual void update() override {
        vel += accel;
        pos += vel;
        accel *= 0.f;
    }
    virtual void set_pos(const float x, const float y) override {}
    void set_max_speed(float max){
        max_speed = max;
    }
    float get_radius(){
        return radius * r_scale;
    }
    void set_scale(float s){ r_scale = s; }
    virtual tVec2f get_vel(){ return vel; }
protected:
    tVec2f pos = {0.f,0.f};
    tVec2f accel = {0.f,0.f};
    tVec2f vel = {0.f,0.f};
    float max_speed = 500.f; // changable
    float mass = 1.f;
    float r_scale = 0.25f;
private:
    float get_speed(){
        return std::sqrt((vel.x * vel.x) + (vel.y * vel.y));
    }
    float radius = 1.f; // .f just tells the compiler that this number is a float. otherwise the compiler has to typecast during compilation
    float width = radius;
    float height = radius;
};

class spinner : public mover {
    /// a mover that rotates around its own center.
    /// spin_speed controls degrees per frame — positive = clockwise, negative = counter-clockwise.
    /// inherits all mover physics (pos, vel, accel, apply_force) so it can move AND spin at the same time.
public:
    void update() override {
        mover::update(); // handles pos/vel/accel
        angle += spin_speed;
    }
    void set_spin(float degrees_per_frame){
        spin_speed = degrees_per_frame;
    }
    float get_angle(){
        return angle;
    }
protected:
    float angle = 0.f;
    float spin_speed = 1.f; // degrees per frame
};


// not entirely necessary
class background : public drawable{
public:
    void setup() override {
        set_color(255, 255, 255);
    }
    void draw() override {
        
    }
};

class glob : public mover {
public:
    glob(){
        for (size_t i = 0; i < num_vert; i++){
            float angle = (TWO_PI/num_vert) * i;
            _2dVertices.append({std::cos(angle), std::sin(angle)}); // unit circle — r_scale applied at draw time
        }
    }
    void init(std::shared_ptr<ofxBox2d> world) override {
        float cx = pos.x;
        float cy = pos.y;
        float radius = std::min(bounds.x, bounds.y) * 0.5f * get_radius();
        physics = std::make_unique<soft_body>(world, 5.f, 0.01f);
        physics->setup(cx, cy, radius, num_vert);
    }
    void set_pos(const float x, const float y) override { // normalized 0-1
        norm_pos = {x, y};
        pos = scale_to_size(norm_pos);
    }
    void set_bounds(const float width, const float height) override {
        bounds.x = width;
        bounds.y = height;
        pos = scale_to_size(norm_pos); // recompute pixel pos from stored norm
    }
    ~glob(){
    }
    ofPath make_path(){
        ofPath p;
        if(bounds.x == 0 || bounds.y == 0) return p;
        float uniform_scale = std::min(bounds.x, bounds.y) * 0.5f * get_radius(); // get_radius() includes r_scale
        for (size_t i = 0; i < num_vert; i++){
            float x = _2dVertices[i].x * uniform_scale + pos.x;
            float y = _2dVertices[i].y * uniform_scale + pos.y;
            if(i == 0){
                p.moveTo(x,y);
            }else{
                p.lineTo(x,y);
            }
        }
        p.close();
        return p;
    }
    void draw() override {
        ofPath p;
        if(physics){
            // physics positions
            auto positions = physics->get_positions();
            for(size_t i = 0; i < positions.size(); i++){
                if(i == 0){
                    p.moveTo(positions[i]);
                }else{
                    p.lineTo(positions[i]);
                }
            }
        }else{
            // if there's no physics for some reason.
            p = make_path();
        }
        p.close();
        p.setFilled(true);
        p.setFillColor(color);
        p.setStrokeWidth(3);
        p.setStrokeColor(ofColor(1, 1, 1));
        p.draw();
    }
    
    void update() override {
        mover::update();
        if(physics){
            physics->check_bounds(bounds.x,bounds.y);
        }
    }
    tVec2f get_pos() override {
        if(physics){
            auto positions = physics->get_positions();
            if(!positions.empty()){
                ofVec2f sum(0, 0);
                for(auto& p : positions) sum += p;
                sum /= (float)positions.size();
                return {sum.x, sum.y};
            }
        }
        return pos;
    }
    tVec2f get_vel() override {
        if(physics){
            ofVec2f v = physics->get_velocity();
            return {v.x, v.y};
        }
        return mover::get_vel();
    }
protected:
    tVec2f bounds;
    size_t num_vert = 5; // keep low — Box2D struggles to solve >20 joints per blob under force
private:
    tVec2f scale_to_size(const tVec2f pos){
        return {(pos.x * bounds.x),(pos.y * bounds.y)};
    }

    tVec2f norm_pos = {0.f, 0.f}; // normalized 0-1, preserved across resizes
    float scale = 1.f;
    d_array<tVec2f> _2dVertices;
};

class static_glob : public glob {
    /// a glob that never moves — uses static_body physics so nothing can push it.
    /// spins its visual shape each frame around its center.
public:
    void init(std::shared_ptr<ofxBox2d> world) override {
        float cx     = pos.x;
        float cy     = pos.y;
        float radius = std::min(bounds.x, bounds.y) * 0.5f * get_radius();
        physics = std::make_unique<spinning_body>(world, spin_speed);
        physics->setup(cx, cy, radius, num_vert);
    }

    void update() override {
        angle += spin_speed;
    }

    void draw() override {
        ofPushMatrix();
        ofTranslate(pos.x, pos.y);
        ofRotateDeg(angle);
        ofTranslate(-pos.x, -pos.y);
        ofPath p = make_path();
        p.setFilled(true);
        p.setFillColor(color);
        p.setStrokeWidth(3);
        p.setStrokeColor(ofColor(0, 0, 0));
        p.draw();
        ofPopMatrix();
    }

    void set_spin(float s) {
        spin_speed = s;
        if(physics){
            auto* sb = dynamic_cast<spinning_body*>(physics.get());
            if(sb) sb->set_angular_velocity(s);
        }
    }
    void set_scale(float s){ r_scale = s; } // controls radius — default 0.25, larger = bigger

private:
    float angle      = 0.f;
    float spin_speed = 1.8f;
};

class data_display : public drawable {
public:
    data_display(){
        physics = nullptr;
        text.load("./assets/InterVariable.ttf", 20);
    }
    void bind_to_drawable(std::shared_ptr<drawable> d) override{
        to_bind = d;
    }
    void set_pos(const float x, const float y) override{
        pos.x = x;
        pos.y = y;
    }
    std::string get_info() override {
        if(!to_bind) return "";
        tVec2f p = to_bind->get_pos();
        std::string output = "";
        output += "pos_x: " + std::to_string(p.x) + "\n";
        output += "pos_y: " + std::to_string(p.y) + "\n";
        auto* m = dynamic_cast<mover*>(to_bind.get());
        if(m){
            tVec2f v = m->get_vel();
            output += "vel_x: " + std::to_string(v.x) + "\n";
            output += "vel_y: " + std::to_string(v.y) + "\n";
        }
        return output;
    }
    void draw() override{
        if(!to_bind) return;
        ofPushStyle();
        ofSetColor(0, 0, 0);
        tVec2f p = to_bind->get_pos();
        text.drawString(get_info(), p.x, p.y);
        ofPopStyle();
    }
private:
    ofTrueTypeFont text;
    tVec2f pos = {0.f,0.f};
    std::shared_ptr<drawable> to_bind = nullptr;
};

class component { // abstract
public:
    virtual void setup(){}
    virtual void update(){}
    virtual void draw(){
        for(auto& m : members){ // uses std::pair on return
            m.second->draw(); // map will return a pair. second grabs the actual object.
        }
    }
    virtual void set_size(const ofVec2f& s){
        width = s.x;
        height = s.y;
    }
    virtual void set_size(const tVec2f& s){
        width = s.x;
        height = s.y;
    }
    virtual void set_size(const float& w, const float& h){
        width = w;
        height = h;
    }
    void add_member(std::string name, std::shared_ptr<drawable> member){
        named_members[name] = member; // auto allocates.
    }
    virtual void draw_order(){} // this will need to be called ONCE during setup to set the draw order. you could also call it more than once. for any reason.

protected:
    std::map<int, std::shared_ptr<drawable>> members; // basically a python dict. for all drawable things.
    std::unordered_map<std::string, std::shared_ptr<drawable>> named_members;
    float width;
    float height;
private:
};

class canvas : public component {
public:
    canvas(){ // add new components here
        add_member("glob",        std::make_shared<glob>());
        add_member("glob2",       std::make_shared<glob>());
        add_member("static_glob", std::make_shared<static_glob>());
        add_member("data_display", std::make_shared<data_display>());
        add_member("data_display2", std::make_shared<data_display>());
        draw_order();
    }
    ~canvas(){}
    void set_size(const float& w, const float& h) override {
        width = w;
        height = h;
        named_members["glob"]->set_bounds(width, height);
        named_members["glob"]->set_pos(0.85f, 0.5f);
        named_members["glob2"]->set_bounds(width, height);
        named_members["glob2"]->set_pos(0.25f, 0.25f);
        auto g1 = std::dynamic_pointer_cast<mover>(named_members["glob"]);
        auto g2 = std::dynamic_pointer_cast<mover>(named_members["glob2"]);
        if(g1) g1->set_scale(0.25f); // change to resize globs
        if(g2) g2->set_scale(0.25f);
        named_members["static_glob"]->set_bounds(width, height);
        named_members["static_glob"]->set_pos(0.5f, 1.2f); // center-bottom
        auto sg = std::dynamic_pointer_cast<static_glob>(named_members["static_glob"]);
        if(sg) sg->set_scale(1.5f); // change this to resize the static_glob
        binds();
    }
    void draw_order() override{
        members[0] = named_members["static_glob"];
        members[1] = named_members["glob"];
        members[2] = named_members["glob2"];
        members[3] = named_members["data_display"];
        members[4] = named_members["data_display2"];
    }
    void update() override{
        for(auto& m : members)
            m.second->update();
    }
    void init(std::shared_ptr<ofxBox2d> world){
        for(auto& m : members){
            m.second->init(world);
        }
    }
    void resized(const float& w, const float& h){
        width = w;
        height = h;
        named_members["glob"]->set_bounds(width, height);
        named_members["glob2"]->set_bounds(width, height);
        named_members["static_glob"]->set_bounds(width, height);
    }
    void binds(){
        named_members["data_display"]->set_pos(20.f, 20.f);
        named_members["data_display"]->bind_to_drawable(named_members["glob"]);
        named_members["data_display2"]->set_pos(20.f, 20.f);
        named_members["data_display2"]->bind_to_drawable(named_members["glob2"]);
    }
private:
};
}
