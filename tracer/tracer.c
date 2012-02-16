#include "tracer.h"

vector aaSteps[18];

void initTracer() {
	int i = 0;
	aaSteps[i++] = vectormake( 0.0f,  0.0f, 0.0f);
	aaSteps[i++] = vectormake(-0.45f,  0.45f, 0.0f);
	aaSteps[i++] = vectormake( 0.45f, -0.45f, 0.0f);
	aaSteps[i++] = vectormake( 0.45f,  0.45f, 0.0f);
	aaSteps[i++] = vectormake(-0.45f, -0.45f, 0.0f);
	aaSteps[i++] = vectormake( 0.0f,  0.45f, 0.0f);
	aaSteps[i++] = vectormake( 0.0f, -0.45f, 0.0f);
	aaSteps[i++] = vectormake( 0.45f,  0.0f, 0.0f);
	aaSteps[i++] = vectormake(-0.45f,  0.0f, 0.0f);
	aaSteps[i++] = vectormake( 0.0f,  0.0f, 0.0f);
	aaSteps[i++] = vectormake(-0.9f,  0.9f, 0.0f);
	aaSteps[i++] = vectormake( 0.9f, -0.9f, 0.0f);
	aaSteps[i++] = vectormake( 0.9f,  0.9f, 0.0f);
	aaSteps[i++] = vectormake(-0.9f, -0.9f, 0.0f);
	aaSteps[i++] = vectormake( 0.0f,  0.9f, 0.0f);
	aaSteps[i++] = vectormake( 0.0f, -0.9f, 0.0f);
	aaSteps[i++] = vectormake( 0.9f,  0.0f, 0.0f);
	aaSteps[i++] = vectormake(-0.9f,  0.0f, 0.0f);
}

float findsphere(scene s, ray r, sphere *sph) {
	int i, sphi = -1;
	float distance = 20000.0;

	for (i = 0; i < (int)s.numSpheres; i++)
	{
		float t = spherehit(s.spheres[i], r);
		if (t < distance && t > 0.0)
		{
			distance = t;
			sphi = i;
		}
	}

	if (sphi >= 0)
	{
		*sph = s.spheres[sphi];
		return distance;
	}

	return -1.0f;
}

vector raytrace(scene *curscene, ray r, float c)
{
	int i, depth = 0;
	vector color = curscene->bgcolor;

	do {
		depth++;

		sphere cursphere, lightsphere;
		float distance = findsphere(*curscene, r, &cursphere);
		if (distance < 0) break;

		vector hitpoint = add(r.start, smul(r.direction, distance));
		vector normal = unit(sub(hitpoint, cursphere.position));

		for (i = 0; i < (int)curscene->numLights; i++)
		{
			light lightsource = curscene->lights[i];
			vector lightvector = sub(lightsource.position, hitpoint);

			if (dot(normal, lightvector) < 0)
				continue;

			int inShadow = 0;
			ray lightray = raymake(hitpoint, lightsource.position);

			if (findsphere(*curscene, lightray, &lightsphere) >= 0)
				inShadow = 1;

			if (!inShadow)
			{
				float lambert = dot(lightray.direction, normal) * c;
				color.x += lightsource.color.x * cursphere.color.x * lambert;
				color.y += lightsource.color.y * cursphere.color.y * lambert;
				color.z += lightsource.color.z * cursphere.color.z * lambert;
			}
		}

		c *= cursphere.reflection;
		float ref = 2.0f * (dot(r.direction, normal));
		r.start = hitpoint;
		r.direction = sub(r.direction, smul(normal, ref));

	} while(depth < (int)curscene->rayDepth);

	return color;
}

/*
vector pixelraytrace(scene *curscene, int x, int y)
{
	vector color = vectormake(0, 0, 0);

	int antiAliasing = (int)curscene->antiAliasing;
	int resolution = (int)curscene->resolution;

	float aainterval = 1.0f / antiAliasing;
	float aaweight = antiAliasing == 1 ? 1.0f : 1.0f / pow(antiAliasing + 1.0f, 2.0f);
	int aaboundary = antiAliasing / 2;

	int ifrag, jfrag;

	for (ifrag = -aaboundary; ifrag <= aaboundary; ifrag++)
	{
		for (jfrag = -aaboundary; jfrag <= aaboundary; jfrag++)
		{
			float scenex = ((x + ifrag * aainterval) - (resolution / 2.0f)) / resolution;
			float sceney = ((y + jfrag * aainterval) - (resolution / 2.0f)) / resolution;

			ray pixelRay = raymake(curscene->camera, vectormake(scenex, sceney, 0.0));
			vector pixelColor = raytrace(curscene, pixelRay, 1.0);

			color = add(color, smul(pixelColor, aaweight));
		}
	}

	return color;
}
*/

vector pixelraytraceaa(scene *curscene, int x, int y, int aastep)
{
	aastep = aastep % 18;
	vector aaStep = aaSteps[aastep];
	int resolution = (int)curscene->resolution;

	float scenex = ((x + aaStep.x) - (resolution / 2.0f)) / resolution;
	float sceney = ((y + aaStep.y) - (resolution / 2.0f)) / resolution;

	ray pixelRay = raymake(curscene->camera, vectormake(scenex, sceney, 0.0));
	vector pixelColor = raytrace(curscene, pixelRay, 1.0);

	return pixelColor;
}

vector pixelraytrace(scene *curscene, int x, int y)
{
	vector color = vectormake(0, 0, 0);

	int antiAliasing = ((int)curscene->antiAliasing) % 18;
	int resolution = (int)curscene->resolution;
	float aaweight = 1.0f / antiAliasing;
	int i;

	for (i = 0; i < antiAliasing; i++)
	{
		vector aaStep = aaSteps[i];

		float scenex = ((x + aaStep.x) - (resolution / 2.0f)) / resolution;
		float sceney = ((y + aaStep.y) - (resolution / 2.0f)) / resolution;

		ray pixelRay = raymake(curscene->camera, vectormake(scenex, sceney, 0.0));
		vector pixelColor = raytrace(curscene, pixelRay, 1.0);

		color = add(color, smul(pixelColor, aaweight));
	}

	return color;
}

/*
int main()
{
	scene *curscene = sceneLoad();
	TGAFILE *tga = openTGA(curscene);

	int i, j;

	for (j = 0; j < (int)curscene->resolution; j++)
	{
		for (i = 0; i < (int)curscene->resolution; i++)
		{
			vector color = pixelraytrace(curscene, i, j);
			writeTGAColor(tga, curscene, i, j, color.x, color.y, color.z);
		}
	}

	sceneFree(curscene);
	closeTGA(tga);

	return 0;
}
*/

