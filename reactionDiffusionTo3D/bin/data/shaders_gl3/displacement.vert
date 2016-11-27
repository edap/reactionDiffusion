#version 150

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelMatrix;
//uniform mat4 normalMatrix;

//used in the normal map
float xOffset = 1.0;
float yOffset = 1.0;

uniform sampler2DRect tex0;
uniform float displaceAmount;
uniform float useNormalMap;

in vec4 position;
in vec2 texcoord;
in vec4 normal;

out vec2 vTexCoord;
out vec4 vPosition;
out vec3 vNormal;

// normally, when doing normal mapping, the normal map is given throug a texture.
// Here it is calculated on the fly. See normals.frag, normals.vert, and try to
// apply the shaderNormalMap to debug it.
// this calculations comes from https://github.com/patriciogonzalezvivo/ofxFX/blob/master/src/operations/ofxNormals.h
vec3 vFromNormalMap(){
    float sCoord		= texcoord.s;
    float tCoord		= texcoord.t;

    float center		= texture(tex0, vec2( sCoord, tCoord ) ).r;
    float topLeft	= texture(tex0, vec2(sCoord - xOffset	, tCoord - yOffset	) ).r;
    float left		= texture(tex0, vec2(sCoord - xOffset	, tCoord			) ).r;
    float bottomLeft	= texture(tex0, vec2(sCoord - xOffset	, tCoord + yOffset	) ).r;
    float top		= texture(tex0, vec2(sCoord			, tCoord - yOffset	) ).r;
    float bottom		= texture(tex0, vec2(sCoord			, tCoord + yOffset	) ).r;
    float topRight	= texture(tex0, vec2(sCoord + xOffset	, tCoord - yOffset	) ).r;
    float right		= texture(tex0, vec2(sCoord + xOffset	, tCoord			) ).r;
    float bottomRight= texture(tex0, vec2(sCoord + xOffset	, tCoord + yOffset	) ).r;

    float dX = topRight + 2.0 * right + bottomRight - topLeft - 2.0 * left - bottomLeft;
    float dY = bottomLeft + 2.0 * bottom + bottomRight - topLeft - 2.0 * top - topRight;

    vec3 N = normalize( vec3( dX, dY, 0.01));

    // this is needed to transform a vector that goes from -1 to 1 to a vector that
    // goes from 0 to 1. Normals vector, goes from -1 to 1, not from 0 to 1
    // It is like to say
    // vec3 N = N * 0.5 + 0.5; // transforms from [-1,1] to [0,1]
    N *= 0.5;
    N += 0.5;
    return N;
}

void main() {

    vTexCoord = texcoord;
    // the normal map calculation it is still a bit buggy
    if (useNormalMap > 0.5) {
        //all this part comes from http://learnopengl.com/#!Advanced-Lighting/Normal-Mapping
        // Now we go in Tangent Space
        // The Tangent Space is the coordinate space that the normals in a normal map are in.
        // this is an approximative method to obtain the tangent from the normal.
        // it comes from http://www.geeks3d.com/20130122/normal-mapping-without-precomputed-tangent-space-vectors/
        vec3 tangent;
        vec3 bitangent;
        vec3 c1 = cross(normal.xyz, vec3(0.0, 0.0, 1.0));
        vec3 c2 = cross(normal.xyz, vec3(0.0, 1.0, 0.0));
        if (length(c1) > length(c2))
            tangent = c1;
        else
            tangent = c2;

        tangent     = normalize(tangent);
        bitangent   = normalize(cross(normal.xyz, tangent));

        // now we need what it is called a TBN matrix.
        // A TBN matrix converts normals from the normal map (in Tangent Space) to Model Space.
        // A TBN matrix looks like this:
        //    tangent = normalize(tangent);
        //    bitangent = normalize(cross(normal, tangent));
        //    mat3 tbn = mat3( tangent, bitangent, normal );
        // but what we need is to convert the tangen space to world space, in a way that we
        // we can calculate the light direction
        vec3 T = normalize(vec3(modelMatrix * vec4(tangent,   0.0)));
        vec3 B = normalize(vec3(modelMatrix * vec4(bitangent, 0.0)));
        vec3 N = normalize(vec3(modelMatrix * normal));
        mat3 TBN = mat3(T, B, N);

        // back to vector that goes from -1 to +1
        vec3 normalFromNormalMap = normalize(vFromNormalMap() * 2.0 -1.0);
        vNormal = normalize(TBN * normalFromNormalMap);
        vPosition = modelViewProjectionMatrix * vec4( position.xyz, 1.0 ) ;
    }else{
        vNormal = normal.xyz;
        //if the normal map is not used, vertex displacement is available
        vec4 color = texture(tex0, vTexCoord);
        float displacement = + displaceAmount * (color.g + color.b);
        // move the position along the normal and transform it
        vec3 newPosition = position.xyz + vNormal * displacement;
        vPosition = modelViewProjectionMatrix * vec4( newPosition, 1.0 );
    };

    gl_Position = vPosition;
    //gl_Position = modelViewProjectionMatrix * position;
}