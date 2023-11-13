#include "CollisionManager.h"
#include "Collision.h"
#include "MeshCollider.h"
#include "RayCast.h"
#include "QueryCallBack.h"
#include <cassert>

#include "Object3D.h"

CollisionManager* CollisionManager::GetInstance()
{
    static CollisionManager instance;
    return &instance;
}

void CollisionManager::CheckAllCollisions()
{
    std::forward_list<std::unique_ptr<BaseCollider>>::iterator itB;

    auto itA = colliders.begin();

    for (; itA != colliders.end(); ++itA) {
        itB = itA;
        ++itB;
        for (; itB != colliders.end(); ++itB) {
            BaseCollider* colA = itA->get();
            BaseCollider* colB = itB->get();

            if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
                colB->GetShapeType() == COLLISIONSHAPE_SPHERE) {
                //  球同士の当たり判定
                Sphere* sphereA = dynamic_cast<Sphere*>(colA);
                Sphere* sphereB = dynamic_cast<Sphere*>(colB);
                Vector3D inter;
                if (Collision::CheckSphere2Sphere(*sphereA, *sphereB, &inter)) {
                    CollisionInfo colBInfo(colB->GetObject3D(), colB, inter);
                    CollisionInfo colAInfo(colA->GetObject3D(), colA, inter);
                    colA->OnCollision(colBInfo);
                    colB->OnCollision(colAInfo);
                }
            }
            else if (colA->GetShapeType() == COLLISIONSHAPE_PLANE &&
                colB->GetShapeType() == COLLISIONSHAPE_SPHERE) {
                Plane* plane = dynamic_cast<Plane*>(colA);
                Sphere* sphere = dynamic_cast<Sphere*>(colB);
                Vector3D inter;
                if (Collision::CheckSphere2Plane(*sphere, *plane, &inter)) {
                    CollisionInfo colBInfo(colB->GetObject3D(), colB, inter);
                    CollisionInfo colAInfo(colA->GetObject3D(), colA, inter);
                    colA->OnCollision(colBInfo);
                    colB->OnCollision(colAInfo);
                }
            }
            else if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
                colB->GetShapeType() == COLLISIONSHAPE_PLANE) {
                Plane* plane = dynamic_cast<Plane*>(colB);
                Sphere* sphere = dynamic_cast<Sphere*>(colA);
                Vector3D inter;
                if (Collision::CheckSphere2Plane(*sphere, *plane, &inter)) {
                    CollisionInfo colBInfo(colB->GetObject3D(), colB, inter);
                    CollisionInfo colAInfo(colA->GetObject3D(), colA, inter);
                    colA->OnCollision(colBInfo);
                    colB->OnCollision(colAInfo);
                }
            }
            else if (colA->GetShapeType() == COLLISIONSHAPE_MESH &&
                colB->GetShapeType() == COLLISIONSHAPE_SPHERE) {
                MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
                Sphere* sphere = dynamic_cast<Sphere*>(colB);
                Vector3D inter;
                if (meshCollider->CheckCollisionSphere(*sphere, &inter)) {
                    CollisionInfo colBInfo(colB->GetObject3D(), colB, inter);
                    CollisionInfo colAInfo(colA->GetObject3D(), colA, inter);
                    colA->OnCollision(colBInfo);
                    colB->OnCollision(colAInfo);
                }
            }
            else if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE &&
                colB->GetShapeType() == COLLISIONSHAPE_MESH) {
                MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
                Sphere* sphere = dynamic_cast<Sphere*>(colA);
                Vector3D inter;
                if (meshCollider->CheckCollisionSphere(*sphere, &inter)) {
                    CollisionInfo colBInfo(colB->GetObject3D(), colB, inter);
                    CollisionInfo colAInfo(colA->GetObject3D(), colA, inter);
                    colA->OnCollision(colBInfo);
                    colB->OnCollision(colAInfo);
                }
            }
        }
    }
}

bool CollisionManager::Raycast(const Ray& ray, RayCast* hitinfo, float maxDistance)
{
    return Raycast(ray, 0xFFFF, hitinfo, maxDistance);
}

bool CollisionManager::Raycast(const Ray& ray, unsigned short attribute, RayCast* hitinfo, float maxDistance)
{
    bool ans = false;
    auto itr = colliders.begin();
    std::forward_list<std::unique_ptr<BaseCollider>>::iterator itr_hit;
    float distance = maxDistance;
    Vector3D inter;

    for (; itr != colliders.end(); ++itr) {
        BaseCollider* colA = itr->get();

        if (!(colA->attribute & attribute)) continue;

        if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE) {
            Sphere* sphere = dynamic_cast<Sphere*>(colA);
            float tempDis;
            Vector3D tempInter;

            if (!Collision::CheckRay2Sphere(ray, *sphere, &tempDis, &tempInter)) continue;
            if (tempDis >= distance) continue;

            ans = true;
            distance = tempDis;
            inter = tempInter;
            itr_hit = itr;
        }
        else if (colA->GetShapeType() == COLLISIONSHAPE_PLANE) {
            Plane* plane = dynamic_cast<Plane*>(colA);
            float tempDis;
            Vector3D tempInter;

            if (!Collision::CheckRay2Plane(ray, *plane, &tempDis, &tempInter)) continue;
            if (tempDis >= distance) continue;

            ans = true;
            distance = tempDis;
            inter = tempInter;
            itr_hit = itr;
        }
        else if (colA->GetShapeType() == COLLISIONSHAPE_MESH) {
            MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);

            float tempDistance;
            Vector3D tempInter;
            if (!meshCollider->CheckCollisionRay(ray, &tempDistance, &tempInter)) continue;
            if (tempDistance >= distance) continue;

            ans = true;
            distance = tempDistance;
            inter = tempInter;
            itr_hit = itr;
        }
    }

    if (ans && hitinfo) {
        hitinfo->distance = distance;
        hitinfo->inter = inter;
        hitinfo->collider = itr_hit->get();
        hitinfo->object = hitinfo->collider->GetObject3D();
    }

    return ans;
}

void CollisionManager::QuerySphere(const Sphere& sphere, QueryCallBack* callback, unsigned short attribute)
{
    assert(callback);

    for (auto it = colliders.begin(); it != colliders.end(); ++it) {
        BaseCollider* col = it->get();

        if (!(col->attribute & attribute)) continue;

        // 球
        if (col->GetShapeType() == COLLISIONSHAPE_SPHERE) {
            Sphere* sphereB = dynamic_cast<Sphere*>(col);

            Vector3D tempInter;
            Vector3D tempReject;
            if (!Collision::CheckSphere2Sphere(sphere, *sphereB, &tempInter, &tempReject)) continue;

            QueryHit info;
            info.collider = col;
            info.object = col->GetObject3D();
            info.inter = tempInter;
            info.reject = tempReject;

            if (!callback->OnQueryHit(info)) return;
        }
        // メッシュ
        else if (col->GetShapeType() == COLLISIONSHAPE_MESH) {
            MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(col);

            Vector3D tempInter;
            Vector3D tempReject;
            if (!meshCollider->CheckCollisionSphere(sphere, &tempInter, &tempReject)) continue;

            QueryHit info;
            info.collider = col;
            info.object = col->GetObject3D();
            info.inter = tempInter;
            info.reject = tempReject;

            if (!callback->OnQueryHit(info)) return;
        }
    }
}
