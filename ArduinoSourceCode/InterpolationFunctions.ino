int InterpolationInteger(int x1, int x2, int y1, int y2, int xIn){
    int yOut;
    yOut = (xIn-x1)*(y2-y1)/(x2-x1)+y1;
}

float InterpolationFloat(float x1, float x2, float y1, float y2, float xIn){
    float yOut;
    yOut = (xIn-x1)*(y2-y1)/(x2-x1)+y1;
}